local M = {}

function M.apply(target, canonical, localized)
  for i = 1, #canonical do
    local aliases = localized[i]
    local source = target[canonical[i]]
    if source ~= nil and aliases then
      if type(aliases) == "string" then
        target[aliases] = source
      else
        for _, alias in ipairs(aliases) do
          target[alias] = source
        end
      end
    end
  end
end

return M
