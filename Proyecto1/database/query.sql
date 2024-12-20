CREATE DATABASE IF NOT EXISTS Proyecto1;

USE Proyecto1;

CREATE TABLE IF NOT EXISTS ram (
    id INT UNSIGNED NOT NULL AUTO_INCREMENT,
    ip VARCHAR(15) NOT NULL,
    total_ram FLOAT NOT NULL,
    free_ram FLOAT NOT NULL,
    used_ram FLOAT NOT NULL,
    percentage_ram FLOAT NOT NULL,
    date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS ram (
    id INT UNSIGNED NOT NULL AUTO_INCREMENT,
    ip VARCHAR(39) NOT NULL,
    total_ram FLOAT NOT NULL,
    free_ram FLOAT NOT NULL,
    used_ram FLOAT NOT NULL,
    percentage_ram FLOAT NOT NULL,
    date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS cpu (
    id INT UNSIGNED NOT NULL AUTO_INCREMENT,
    ip VARCHAR(15) NOT NULL,
    percentage_cpu FLOAT NOT NULL,
    date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS tasks (
    id INT UNSIGNED NOT NULL AUTO_INCREMENT,
    ip VARCHAR(15) NOT NULL,
    pid INT NOT NULL,
    name VARCHAR(50) NOT NULL,
    status INT NOT NULL,
    user INT NOT NULL,
    ram FLOAT,
    father INT NOT NULL,
    date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id)
);

SELECT
    percentage_ram
FROM
    ram
WHERE
    ip = hostIp;

SELECT
    percentage_cpu
FROM
    cpu
WHERE
    ip = hostIp;

SELECT
    percentage_ram,
    date
FROM
    ram
WHERE
    ip = hostIP;

SELECT
    percentage_cpu,
    date
FROM
    cpu
WHERE
    ip = hostIP;

SELECT
    pid,
    name,
    status,
    user,
    father
FROM
    tasks
WHERE
    ip = hostIp;

DELETE FROM
    tasks
WHERE
    ip = hostIp;