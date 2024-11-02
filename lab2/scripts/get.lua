math.randomseed(998244353)

request = function()
    headers = {}
    headers["Content-Type"] = "application/json"
    body = ''
    return wrk.format("GET", "/user?id=" .. math.random(1, 100), headers, body)
end
