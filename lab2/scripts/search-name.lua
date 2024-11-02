math.randomseed(998244353)

function genCapitalChar()
    return string.char(math.random(65, 90))
end

request = function()
    headers = {}
    headers["Content-Type"] = "application/json"
    body = ''
    return wrk.format("GET", "/user/search/name?first_name=" .. genCapitalChar() .. "&last_name=" .. genCapitalChar(), headers, body)
end
