#include "Notes.h"

sumi::PluginInterface* createNotesApp(sumi::PluginRenderer& r) {
  return new sumi::NotesApp(r);
}