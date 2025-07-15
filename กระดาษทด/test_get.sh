HOST=localhost
PORT=8080

curl -v -X GET "http://$HOST:$PORT/search?q=webserv&lang=en" \
     -H "User-Agent: curl/7.68.0" \
     -H "Accept: */*" \
     -H "Cookie: PHPSESSID=xyz789"


# bash test_get.sh
# make t22