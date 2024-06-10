HTTP server performance history:

Before optimalistation
```
Running 10s test @ http://127.0.0.1:8181/
  1 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     8.75ms   14.48ms 109.85ms   89.40%
    Req/Sec    16.24k    12.43k   30.33k    59.57%
  154413 requests in 10.09s, 0.00B read
  Socket errors: connect 0, read 154413, write 0, timeout 0
Requests/sec:  15311.14
Transfer/sec:       0.00B
```

after optimalisation after wdr test (1 thread, 100 connections, 10s duration)
```
Running 10s test @ http://127.0.0.1:8181/
  1 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     2.22ms    1.47ms  40.14ms   96.64%
    Req/Sec    45.22k     1.80k   50.71k    80.00%
  449959 requests in 10.04s, 131.31MB read
Requests/sec:  44821.95
Transfer/sec:     13.08MB
```
