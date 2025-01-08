math.randomseed(998244353)

request = function()
    headers = {}
    headers["Content-Type"] = "application/json"
    body = ''
    return wrk.format("POST", "/message/user?text=Hello,world!&from_user_id=" .. math.random(1, 100000) .. "&to_user_id=" .. math.random(1, 100000), headers, body)
end
