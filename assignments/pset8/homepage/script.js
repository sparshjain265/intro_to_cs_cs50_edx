function inc(house, p) {
	points = JSON.parse(localStorage.getItem("points"));
	points[house] += p;
	localStorage.setItem("points", JSON.stringify(points));
	display();
}

function display() {
	points = JSON.parse(localStorage.getItem("points"));
	let g = document.querySelector("#gryffindor-points");
	if (g)
		g.innerHTML = points['gryffindor'];
	let h = document.querySelector("#hufflepuff-points");
	if (h)
		h.innerHTML = points['hufflepuff'];
	let r = document.querySelector("#ravenclaw-points");
	if (r)
		r.innerHTML = points['ravenclaw'];
	let s = document.querySelector("#slytherin-points");
	if (s)
		s.innerHTML = points['slytherin'];
}
