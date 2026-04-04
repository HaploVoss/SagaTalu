#include "Notes.h"

sagatalu::PluginInterface* createNotesApp(sagatalu::PluginRenderer& r) {
  return new sagatalu::NotesApp(r);
}