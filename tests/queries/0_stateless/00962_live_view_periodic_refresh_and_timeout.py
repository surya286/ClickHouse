#!/usr/bin/env python3
# Tags: no-replicated-database, no-parallel, no-fasttest

import os
import sys
import time
import signal

CURDIR = os.path.dirname(os.path.realpath(__file__))
sys.path.insert(0, os.path.join(CURDIR, 'helpers'))

from client import client, prompt, end_of_block

log = None
# uncomment the line below for debugging
#log=sys.stdout

with client(name='client1>', log=log) as client1, client(name='client2>', log=log) as client2:
    client1.expect(prompt)
    client2.expect(prompt)

    client1.send('SET allow_experimental_live_view = 1')
    client1.expect(prompt)
    client2.send('SET allow_experimental_live_view = 1')
    client2.expect(prompt)

    client1.send('DROP TABLE IF EXISTS test.lv')
    client1.expect(prompt)
    client1.send("CREATE LIVE VIEW test.lv WITH TIMEOUT 60 AND REFRESH 1"
                 " AS SELECT value FROM system.events WHERE event = 'OSCPUVirtualTimeMicroseconds'")
    client1.expect(prompt)
    client1.send('WATCH test.lv FORMAT JSONEachRow')
    client1.expect(r'"_version":' + end_of_block)
    client1.expect(r'"_version":' + end_of_block)
    client1.expect(r'"_version":' + end_of_block)
    # send Ctrl-C
    client1.send('\x03', eol='')
    match = client1.expect('(%s)|([#\$] )' % prompt)
    if match.groups()[1]:
        client1.send(client1.command)
        client1.expect(prompt)
    # poll until live view table is dropped
    start_time = time.time()
    while True:
        client1.send('SELECT * FROM test.lv FORMAT JSONEachRow')
        client1.expect(prompt)
        if 'Table test.lv doesn\'t exist' in client1.before:
            break
        if time.time() - start_time > 90:
            break
    # check table is dropped
    client1.send('DROP TABLE test.lv')
    client1.expect('Table test.lv doesn\'t exist')
    client1.expect(prompt)
