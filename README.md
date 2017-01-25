# HM-Sensor

Nachbau des "Universalsensors" von Dirk, allerdings mit anderen Sensoren.

Zum Einsatz kommen die günstigen DHT-22 und/oder DS18B20 Sensoren. 
Alternativ kann noch ein SHT-21 (sehr kompakt und genau) verwendet werden (Code hierfür allerdings nicht vorhanden)

Der Sensor verwendet als Untermodul die NewAskSin Library von Trilu. Diese habe ich für meine Bedürfnisse etwas angepaßt, wird aber zur Zeit regelmäßig mit Trilus Code aktualisiert.

**Anpassungen:**

 - Erweiterung für WeatherSensor Messages
 - weitere Verbesserungen beim Energieverbrauch
 - Pairing-Status wird über LED sofort rückgemeldet
 - Stabilität beim Peering mit HM-CC-RT-DN verbessert
 - Einbau eines 32KHz Uhrenquarzes möglich - damit weitere Verbesserung der Übertragungsgenauigkeit zu einem HM-CC-RT-DN und weitere Reduktion des Stromverbrauchs
 - LED beim Senden kann abgeschaltet werden
 - OSCCAL-Register des Atmel kann remote eingestellt werden (damit kann auch ohne Quarz eine gute Genauigkeit der Taktfrequenz erreicht werden)
 - Automatische Watchdog-Kalibrierung
 - Serielle Debug-Ausgabe auf "flush" umgestellt; damit entfallen die teilweise eingebauten schlecht kalkulierbaren delays
 - Batteriemessgenauigkeit auf 1/100V erhöht (ich mag's halt genau)
 - HAVE_DATA Message-Verarbeitung: damit nimmt der Sensor auch ohne Drücken des Config-Buttons Befehle von der Zentrale bei der nächsten Temperaturübermittlung an
 - der eine oder andere Fix (teilweise von Trilu bereits eingebaut)

Schaltplan, Layout und Belichtungsplan gibt's im Unterordner Schematic.
Da ich meine Platinen selber ätze, habe ich hier auf doppelseitiges Layout verzichtet - es gibt dafür ein paar Drahtbrücken.

Als Sendemodul kommt ein CC1101 auf einem günstigen Breakout-Board aus China zum Einsatz.
Fotos vom fertigen Modul gibt's auch noch...

Entwicklungsumgebung: Atmel Studio 7.0

Der Branch "dev" ist mein aktueller Entwicklungszweig - die Version ist evtl. fehlerhaft oder gar nicht compilierbar.

Das Release 1.3 ist das letzte stabile, 1.4 hat zusätzlich Code für die Verwendung eines 32KHz-Quarzes
