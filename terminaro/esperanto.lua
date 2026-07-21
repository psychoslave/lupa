local praprovizo = require("terminaro.praprovizo")
local registry = require("terminaro.registry")

local terminaro = {
  lex = {
    "kaj", {"eksterŝalte", "ekstersxalte"}, "fare", "alie", "alise",
    "hop", "falsa", "por", "funkcie", "tie", "se",
    "el", "loka", "nilo", "ne", "aŭ", "cikle",
    "reŝalte", "tiam", "vera", "ĝis", "dum",
    {"onige", "parte", "pece", "kvociente"},
    {"lige", "kroĉe", "krocxe"},
    "ktp", "egalas", {"almenaŭas", "almenauxas"}, {"maksimumas", "subas", "malsuras", "malalmenaŭas", "malalmenauxas"},
    {"malegalas", "malsamas", "neegalas", "nesamas"},
    {"<nombra>"},
    {"<indukta>"},
    {"<noma>"},
    {"<ĉena>"},
  },

  base = {
    "asertu", "senrubigu", "dosierrulu", "eraravizu", "metatabelodonu", "induktparodonu",
    {"dosiersxargu", "dosierŝargu"}, {"sxargu", "ŝargu"}, {"cxensxargu", "ĉenŝargu"},
    "sekvdonu", "parodonu", {"sxirmvoku", "ŝirmvoku"}, {"printe", "printu"},
    "rektegalu", "rektpesu", "rektdonu", "rektloku", "selektu", "metatabelloku",
    "nombrigu", {"cxenigu", "ĉenigu"}, "tipdonu", {"traktsxirmvoku", "traktŝirmvoku"},
  },

  package = {
    {"teksxargu", "tekŝargu"}, {"dosierindikosercxu", "dosierindikoserĉu"}, {"plenvidu"},
    false, false, false, false, false,
  },

  globals = {
    false,
    "necesu",
  },
}

terminaro.praprovizo = praprovizo

if _G.package ~= nil then
  registry.apply(_G, praprovizo.base, terminaro.base)
  registry.apply(package, praprovizo.package, terminaro.package)
  registry.apply(_G, praprovizo.globals, terminaro.globals)
end

_G.terminaro = terminaro

return terminaro
