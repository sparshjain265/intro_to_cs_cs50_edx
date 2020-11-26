SELECT name FROM stars JOIN people ON person_id = id WHERE movie_id IN
(SELECT movie_id FROM stars JOIN people ON person_id = id WHERE name = 'Kevin Bacon' AND birth = 1958) AND
(NOT (name = 'Kevin Bacon' AND birth = 1958));