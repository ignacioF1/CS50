-- Write a SQL query to list the names of all people who starred in Toy Story.
-- Your query should output a table with a single column for the name of each person.
-- You may assume that there is only one movie in the database with the title Toy Story.

SELECT
name
FROM
people
JOIN stars
ON people.id = stars.person_id
WHERE movie_id IN
(SELECT
id
FROM
movies
WHERE
movies.title = 'Toy Story');