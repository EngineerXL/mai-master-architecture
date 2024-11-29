math.randomseed(998244353)

request = function()
    headers = {}
    headers["Content-Type"] = "application/json"
    body = ''
    return wrk.format("GET", "/chat?id=" .. math.random(1, 1000), headers, body)
end
