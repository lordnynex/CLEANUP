# goquery
use go to executive osqueryi to get the data

##Requirement

* GO 
* osqurey by facebook [github](https://github.com/facebook/osquery)
* GO-gin [github](https://github.com/gin-gonic/gin)

###Run
```bash
go run main.go
```

```go
go install
goquery
```

###Page
```bash
http://localhost:8080/tables
```

###Curl
```bash
curl -i -H "Content-Type: application/json" -X POST \
      -d '{ "user":"leitu", "sql": "SELECT version  FROM kernel_info"}'\
      http://localhost:8080/query
```

###Results

```bash
HTTP/1.1 200 OK
Content-Type: text/plain; charset=utf-8
Date: Tue, 01 Mar 2016 08:12:31 GMT
Content-Length: 27

[
  {"version":"15.3.0"}
]
```


You also can use command way to get the data directly

```bash
 go run query-kernel.go
{"version":"15.3.0"}
```

##ToDo
* <del> SQL section to default value.
* move query-kernel.go to cli way