from esphome.components import sensor
from .sensor import EASEARTDSComponent, to_code, CONFIG_SCHEMA

CODEOWNERS = ["@JochenZhou"]
DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor"]