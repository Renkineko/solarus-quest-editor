-- This script updates all data files of a solarus 1.1 quest
-- into the format of solarus 1.2.
-- Usage:
--   local converter = require("converter_1_1_to_1_2")
--   converter.upgrade(quest_path)

local converter = {}

local function write_info(message)

  io.write(message, "\n")
  io.flush()
end

function converter.convert(quest_path)

  assert(quest_path)

  write_info("Updating your quest " .. quest_path .. " from Solarus 1.1 to Solarus 1.2.")
  write_info("It is recommended to backup your quest files before.")

  -- Convert the quest properties file quest.dat.
  write_info("  Converting the quest properties file...")
  local quest_properties_converter = require("1_1_to_1_2/quest_properties_converter_1_1")
  quest_properties_converter.convert(quest_path)

  -- Convert the resource list file project_db.dat.
  write_info("  Reading the list of resources...")
  local quest_db_converter = require("1_1_to_1_2/quest_db_converter_1_1")
  local resources = quest_db_converter.convert(quest_path)

  -- Convert maps.
  write_info("  Converting maps...")
  local map_converter = require("1_1_to_1_2/map_converter_1_1")
  for _, resource in pairs(resources["map"]) do
    write_info("    Map " .. resource.id .. " (" .. resource.description .. ")")
    map_converter.convert(quest_path, resource.id)
  end
  write_info("  All maps were converted.")

  -- Convert strings.dat files.
  write_info("  Converting strings...")
  local strings_converter = require("1_1_to_1_2/strings_converter_1_1")
  for _, resource in pairs(resources["language"]) do
    write_info("    Language " .. resource.id .. " (" .. resource.description .. ")")
    strings_converter.convert(quest_path, resource.id)
  end
  write_info("  All strings files were converted.")

  -- Create new required sound.
  write_info("  Converting sounds...")
  local sounds_converter = require("1_1_to_1_2/sound_converter_1_1")
  sounds_converter.convert(quest_path)

  write_info("Update successful!")

end

return converter

