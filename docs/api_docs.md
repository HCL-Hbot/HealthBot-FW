API functionaliteit:
- Motors aansturen
| -> config parameters (current, speed, steps per mm, driver id?)
| -> Yaw, Pitch, speed
- Display elementen/objecten aansturen (afzonderlijk aansturen?)
| -> elementen aan/uit zetten
| -> elementen verplaatsen
| -> voorgeanimeerde animaties afspelen (zoals knipperen, vergroten?)
- Ledstrips aansturen
| -> aan/uit
| -> kleur
| -> intensiteit
| -> config parameters
- Radar 
|- > aanwezigheid detectie
|- > streaming (raw) data (gewenst)
|- > config parameters
- Status
|- > Motor status
|- > Led status/HB
|- > Display status
|- > Radar status

/**********************************************************************************************************************/
*													Motor API														  *
/**********************************************************************************************************************/

    Configuratie: M101 D<driver-id> C<current> S<speed> M<steps-per-mm>
        Voorbeeld: M101 D1 C2.5 S1500 M400

    Controleren: M102 Y<yaw> P<pitch> S<speed>
        Voorbeeld: M102 Y90 P45 S1500

/**********************************************************************************************************************/
*													Display API														  *
/**********************************************************************************************************************/
    Toggle: D103 I<element-id> S<on|off>
        Voorbeeld: D103 I2 Soff

    Verplaatsen: D104 I<element-id> X<x-coord> Y<y-coord>
        Voorbeeld: D104 I2 X100 Y200

    Animeren: D105 I<element-id> A<blink|enlarge>
        Voorbeeld: D105 I2 Ablink

/**********************************************************************************************************************/
*													LED Strips API													  *
/**********************************************************************************************************************/
        
    Toggle: L106 S<on|off>
        Voorbeeld: L106 Son

    Kleur Veranderen: L107 C<color>
        Voorbeeld: L107 C#FF0000

    Intensiteit: L108 V<intensity>
        Voorbeeld: L108 V75
        
/**********************************************************************************************************************/
*													Radar API													      *
/**********************************************************************************************************************/      
    Detectie: R109
        Gebruikt voor aanwezigheidsdetectie.

    Data Streaming: R110 S<start|stop>
        Voorbeeld: R110 Sstart

/**********************************************************************************************************************/
*													Status control API												  *
/**********************************************************************************************************************/   
    Status Check: S111 C<motor|led|display|radar>
        Voorbeeld: S111 Cmotor
