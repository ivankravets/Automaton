#include "Atm_bit.hpp"

Atm_bit& Atm_bit::begin( bool default_state /* = false */ ) {
  // clang-format off
  const static STATE_TYPE state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_ON  EVT_OFF  EVT_TOGGLE EVT_INPUT ELSE */
    /* OFF     */    ACT_OFF, ATM_SLEEP,      -1,     ON,      -1,         ON,      OFF,  -1,
    /* ON      */     ACT_ON, ATM_SLEEP,      -1,     -1,     OFF,        OFF,       ON,  -1,
  };
  // clang-format on
  MACHINE::begin( state_table, ELSE );
  _last_state = -1;
  state( default_state ? ON : OFF );
  _indicator = -1;
  cycle();
  return *this;
}

Atm_bit& Atm_bit::indicator( int led, bool activeLow /* = false */ ) {
  _indicator = led;
  _indicatorActiveLow = activeLow;
  pinMode( _indicator, OUTPUT );
  return *this;
}

Atm_bit& Atm_bit::onFlip( bool status, atm_cb_t callback, int idx /* = 0 */ ) {
  _connector[status ? 0 : 1].set( callback, idx );
  return *this;
}

Atm_bit& Atm_bit::onFlip( bool status, Machine& machine, int evt /* = 0 */ ) {
  _connector[status ? 0 : 1].set( &machine, evt );
  return *this;
}

#ifndef TINYMACHINE
Atm_bit& Atm_bit::onFlip( bool status, const char* label, int event /* = 0 */ ) {
  _connector[status ? 0 : 1].set( label, event );
  return *this;
}
#endif

Atm_bit& Atm_bit::onFlip( bool status, TinyMachine& machine, int event /* = 0 */ ) {
  _connector[status ? 0 : 1].set( &machine, event );
  return *this;
}

Atm_bit& Atm_bit::onInput( bool status, atm_cb_t callback, int idx /* = 0 */ ) {
  _connector[status ? 2 : 3].set( callback, idx );
  return *this;
}

Atm_bit& Atm_bit::onInput( bool status, Machine& machine, state_t event /* = 0 */ ) {
  _connector[status ? 2 : 3].set( &machine, event );
  return *this;
}

#ifndef TINYMACHINE
Atm_bit& Atm_bit::onInput( bool status, const char* label, state_t event /* = 0 */ ) {
  _connector[status ? 2 : 3].set( label, event );
  return *this;
}
#endif

Atm_bit& Atm_bit::onInput( bool status, TinyMachine& machine, state_t event /* = 0 */ ) {
  _connector[status ? 2 : 3].set( &machine, event );
  return *this;
}

int Atm_bit::event( int id ) {
  return 0;
}

void Atm_bit::action( int id ) {
  switch ( id ) {
    case ACT_OFF:
      if ( _last_state != -1 ) _connector[_last_state == current ? 3 : 1].push( FACTORY );
      if ( _indicator > -1 ) digitalWrite( _indicator, !LOW != !_indicatorActiveLow );
      _last_state = current;
      return;
    case ACT_ON:
      if ( _last_state != -1 ) _connector[_last_state == current ? 2 : 0].push( FACTORY );
      if ( _indicator > -1 ) digitalWrite( _indicator, !HIGH != !_indicatorActiveLow );
      _last_state = current;
      return;
  }
}

Atm_bit& Atm_bit::trace( Stream& stream ) {
#ifndef TINYMACHINE
  Machine::setTrace( &stream, atm_serial_debug::trace, "EVT_ON\0EVT_OFF\0EVT_TOGGLE\0EVT_INPUT\0ELSE\0OFF\0ON\0INPUTM" );
#endif
  return *this;
}
