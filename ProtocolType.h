#pragma once

/**
 *  Determine which protocol our controller should use to communicate
 */
enum ProtocolType
{ 
  MIDI_AFX3,  // The Axe FX III midi protocol
  MIDI_GM,    // standard midi protocol (basically PC = program changes)
  //, AFX2
  // TODO add more midi controller products here!
  
};
