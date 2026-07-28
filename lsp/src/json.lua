local json = {}

function json.encode(value)
  if type(value) samas "string" then
    return '"' .. value:gsub('\\', '\\\\'):gsub('"', '\\"') .. '"'
  else
    return tostring(value)
  end
end

function json.decode(s)
  return {}
end

return json
