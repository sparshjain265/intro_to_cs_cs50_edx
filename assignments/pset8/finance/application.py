import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, check_password_strength

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # get user_id
    user_id = session["user_id"]

    # get user details
    rows = db.execute("SELECT * FROM users WHERE id = :id", id=user_id)

    # ensure validity
    if len(rows) != 1:
        return apology("Failed to load, please login again", 403)
    user = rows[0]

    # get transaction details
    txns = db.execute(
        "SELECT symbol, sum(shares) AS net_shares FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING net_shares != 0",
        user_id=user_id,
    )

    # initiate response
    response = []

    # initiate total
    total = 0

    # get current market details for each transaction
    for t in txns:
        # get quote
        r = lookup(t["symbol"])

        # ensure valid result
        if not r:
            return apology("Failed to load, try again later", 403)

        tot = r["price"] * t["net_shares"]

        # construct a result
        result = {
            "symbol": t["symbol"],
            "name": r["name"],
            "shares": t["net_shares"],
            "price": usd(r["price"]),
            "total": usd(tot),
        }

        # update total
        total += tot

        # add result to response
        response.append(result)

    # return txns as response
    return render_template(
        "index.html",
        response=response,
        cash=usd(user["cash"]),
        total=usd(total + user["cash"]),
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via GET
    if request.method == "GET":
        # get value
        value = request.args.get("value")
        return render_template("buy.html", value=value)
    # User reached route via POST
    else:
        # get data
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 403)
        # ensure shares was submitted
        elif not shares:
            return apology("must provide number of shares", 403)

        # get user details
        user_id = session["user_id"]
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=user_id)

        # ensure validity
        if len(rows) != 1:
            return apology("Failed to buy, try again later", 403)
        user = rows[0]

        # get quote
        result = lookup(symbol)

        # ensure valid result
        if not result:
            return apology("invalid symbol", 403)

        # check affordability
        shares = int(shares)
        cash = float(user["cash"]) - (result["price"] * shares)
        if cash < 0:
            return apology("can't afford", 403)

        # withdraw cash
        num = db.execute(
            "UPDATE users SET cash = :cash WHERE id = :id", cash=cash, id=user_id
        )

        # check success
        if num != 1:
            return apology("database error, try again later", 403)

        # add transaction
        num = db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, timestamp) VALUES (:user_id, :symbol, :shares, :price, datetime('now'))",
            user_id=user_id,
            symbol=result["symbol"],
            shares=shares,
            price=result["price"],
        )

        # check success
        if num < 1:
            # TODO rollback cash withdrawal done above
            while num != 1:
                num = db.execute(
                    "UPDATE users SET cash = :cash WHERE id = :id",
                    cash=user["cash"],
                    id=user_id,
                )
            return apology("database error, try again later", 403)

        # redirect to homepage
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # get user_id
    user_id = session["user_id"]

    # get user details
    rows = db.execute("SELECT * FROM users WHERE id = :id", id=user_id)

    # ensure validity
    if len(rows) != 1:
        return apology("Failed to load, please login again", 403)
    user = rows[0]

    # get transaction details
    txns = db.execute(
        "SELECT * FROM transactions WHERE user_id = :user_id ORDER BY timestamp DESC",
        user_id=user_id,
    )

    for t in txns:
        t["price"] = usd(t["price"])

    return render_template("history.html", txns=txns)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = :username",
            username=request.form.get("username"),
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change User Password"""

    # User reached route via GET
    if request.method == "GET":
        return render_template("change_password.html")
    # User reached route via POST
    else:
        # get user_id
        user_id = session["user_id"]

        # get user details
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=user_id)

        # ensure validity
        if len(rows) != 1:
            return apology("Failed to load, please login again", 403)
        user = rows[0]

        # get data
        old_password = request.form.get("old_password")
        new_password = request.form.get("new_password")
        confirmation = request.form.get("confirmation")

        # ensure old password was submitted
        if not old_password:
            return apology("must provide current password", 403)
        # ensure new password was submitted
        elif not new_password:
            return apology("must provide new password", 403)
        # ensure confirmation
        elif not confirmation:
            return apology("must confirm new password", 403)

        # check authenticity of current password
        if not check_password_hash(user["hash"], old_password):
            return apology("current password invalid", 403)

        # if passwords are same
        if old_password == new_password:
            return apology("new password cannot be same as old password", 403)

        # check password strength
        if not check_password_strength(new_password):
            return apology(
                "Password must have 8-20 characters, atleast one uppercase letter, atleast one lowercase letter, atleast one digit, and atleast one special character"
            )

        # if confirmation not same
        if new_password != confirmation:
            return apology("passwords must match", 403)

        # get hash of the password
        hash = generate_password_hash(new_password)
        old_password = None
        new_password = None
        confirmation = None

        # Everything correct, update user
        num = db.execute(
            "UPDATE users SET hash=:hash WHERE id=:id", hash=hash, id=user_id
        )

        # If failed
        if num != 1:
            return apology("database error, try again later", 403)

        # redirect user to home page
        return redirect("/")


@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    """Add Cash"""

    # User reached route via GET
    if request.method == "GET":
        return render_template("add_cash.html")
    # user reached route via POST
    else:
        # get user_id
        user_id = session["user_id"]

        # get user details
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=user_id)

        # ensure validity
        if len(rows) != 1:
            return apology("Failed to load, please login again", 403)
        user = rows[0]

        # get data
        cash = request.form.get("cash")

        # check validity
        if not cash:
            return apology("must provide amount to add", 403)

        # get new cash
        cash = float(cash) + float(user["cash"])

        # update cash
        num = db.execute(
            "UPDATE users SET cash=:cash WHERE id=:id", cash=cash, id=user_id
        )

        # if failed
        if num != 1:
            return apology("database error, try again later", 403)

        # redirect user to home page
        return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via GET
    if request.method == "GET":
        return render_template("quote.html")
    # User reached route via POST
    else:
        # get symbol
        symbol = request.form.get("symbol")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 403)

        # get quote
        result = lookup(symbol)

        # ensure valid result
        if not result:
            return apology("invalid symbol", 403)

        # Construct response
        response = (
            "A share of "
            + result["name"]
            + " ("
            + result["symbol"]
            + ") costs "
            + usd(result["price"])
            + "."
        )

        # return response
        return render_template("quoted.html", response=response)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST
    if request.method == "POST":

        # get data
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 403)

        # Ensure password was retyped
        elif not confirmation:
            return apology("must confirm password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = :username", username=username
        )

        # If username exists
        if len(rows) > 0:
            return apology("username already exists", 403)

        # check password strength
        if not check_password_strength(password):
            return apology(
                "Password must have 8-20 characters, atleast one uppercase letter, atleast one lowercase letter, atleast one digit, and atleast one special character"
            )

        # if passwords not same
        if password != confirmation:
            return apology("passwords must match", 403)

        # Get hash of the password
        hash = generate_password_hash(password)
        password = None
        confirmation = None

        # Everything correct, register user
        user_id = db.execute(
            "INSERT INTO users (username, hash) VALUES (:username, :hash)",
            username=username,
            hash=hash,
        )

        # If cannot register
        if not user_id:
            return apology("failed to register, try again later", 403)

        # Log the user in on successful registration
        session["user_id"] = user_id

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via GET
    if request.method == "GET":
        # get user_id
        user_id = session["user_id"]

        # get user details
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=user_id)

        # ensure validity
        if len(rows) != 1:
            return apology("Failed to load, please login again", 403)
        user = rows[0]

        # get transaction details
        txns = db.execute(
            "SELECT symbol, sum(shares) AS net_shares FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING net_shares != 0",
            user_id=user_id,
        )

        # initiate symbols
        symbols = []

        # get symbols
        for t in txns:
            symbols.append(t["symbol"])

        # get value
        value = request.args.get("value")
        return render_template("sell.html", symbols=symbols, value=value)
    # User reached route via POST
    else:
        # get data
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 403)
        # ensure shares was submitted
        elif not shares:
            return apology("must provide number of shares", 403)

        # get user details
        user_id = session["user_id"]
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=user_id)

        # ensure validity
        if len(rows) != 1:
            return apology("Failed to sell, try again later", 403)
        user = rows[0]

        # get transaction details
        txns = db.execute(
            "SELECT symbol, sum(shares) AS net_shares FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING net_shares != 0",
            user_id=user_id,
        )

        # find corresponding entry
        entry = {}
        for t in txns:
            if t["symbol"] == symbol:
                entry = t
                break

        # if entry not found
        if not entry:
            return apology("Failed to sell, try again later", 403)

        # if not enough shares to sell
        shares = int(shares)
        if entry["net_shares"] < shares:
            return apology("Selling more than you own", 403)

        # get quote
        result = lookup(symbol)

        # ensure valid result
        if not result:
            return apology("Failed to sell, try again later", 403)

        # add to cash
        cash = float(user["cash"]) + (result["price"] * shares)
        num = db.execute(
            "UPDATE users SET cash = :cash WHERE id = :id", cash=cash, id=user_id
        )

        # check success
        if num != 1:
            return apology("database error, try again later", 403)

        # add transaction
        num = db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, timestamp) VALUES (:user_id, :symbol, :shares, :price, datetime('now'))",
            user_id=user_id,
            symbol=result["symbol"],
            shares=-shares,
            price=result["price"],
        )

        # check success
        if num < 1:
            # TODO rollback cash deposite done above
            while num != 1:
                num = db.execute(
                    "UPDATE users SET cash=:cash WHERE id=:id",
                    cash=user["cash"],
                    id=user_id,
                )
            return apology("database error, try again later", 403)

        # redirect to homepage
        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
