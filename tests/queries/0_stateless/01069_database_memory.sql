-- Tags: no-parallel

DROP DATABASE IF EXISTS memory_01069;
CREATE DATABASE memory_01069 ENGINE = Memory;
SHOW CREATE DATABASE memory_01069;

CREATE TABLE memory_01069.mt (n UInt8) ENGINE = MergeTree() ORDER BY n;
CREATE TABLE memory_01069.file (n UInt8) ENGINE = File(CSV);

INSERT INTO memory_01069.mt VALUES (1), (2);
INSERT INTO memory_01069.file VALUES (3), (4);

SELECT * FROM memory_01069.mt ORDER BY n;
SELECT * FROM memory_01069.file ORDER BY n;

DROP TABLE memory_01069.mt;
SELECT * FROM memory_01069.mt ORDER BY n; -- { serverError 60 }
SELECT * FROM memory_01069.file ORDER BY n;

SHOW CREATE TABLE memory_01069.mt; -- { serverError 60 }
SHOW CREATE TABLE memory_01069.file;

DROP DATABASE memory_01069;
