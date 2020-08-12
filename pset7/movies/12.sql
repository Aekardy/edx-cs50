SELECT movies.title FROM movies WHERE movies.id IN (SELECT movies.id FROM movies JOIN stars ON stars.movie_id = movies.id JOIN people ON stars.person_id = people.id WHERE people.name = 'Helena Bonham Carter') AND movies.id IN (SELECT movies.id FROM movies JOIN stars ON stars.movie_id = movies.id JOIN people ON stars.person_id = people.id WHERE people.name = 'Johnny Depp');