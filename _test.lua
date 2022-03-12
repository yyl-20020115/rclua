#!../lua
-- $Id: testes/all.lua $
-- See Copyright Notice at the end of this file

local version = "Lua 5.4"
if _VERSION ~= version then
  io.stderr:write("This test suite is for ", version,
                  ", not for ", _VERSION, "\nExiting tests")
  return
end
