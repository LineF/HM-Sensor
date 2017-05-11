# HM-Sensor

Nachbau des "Universalsensors" von Dirk, allerdings mit anderen Sensoren.

Zum Einsatz kommen die günstigen DHT-22 und/oder DS18B20 Sensoren. 
Alternativ kann noch ein SHT-21 (sehr kompakt und genau) verwendet werden (Code hierfür allerdings nicht vorhanden)

Der Sensor verwendet als Untermodul die NewAskSin Library von Trilu. Diese habe ich für meine Bedürfnisse etwas angepaßt, wird aber zur Zeit regelmäßig mit Trilus Code aktualisiert.

**Anpassungen:**

 - Erweiterung für WeatherSensor Messages
 - weitere Verbesserungen beim Energieverbrauch
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
Die erste Version wurde mit Eagle erstellt (Unterordner Eagle)
Da ich meine Platinen selber ätze, habe ich hier auf doppelseitiges Layout verzichtet - es gibt dafür ein paar Drahtbrücken.

Die zweite Version wurde mit KiCad erstellt und dann bei einem professionellen Fertiger gefertigt (Unterordner Kicad).
Da gibts auch die entsprechenden Bilder.

Achtung:
Die aktuelle Version ab Mitte Mai 2017 hat statt GDO0 jetzt GDO2 verbunden (Fehler beim Erstellen des Schaltplans).
Dies wurde aber im Communication-Modul bei den Parametern IOCFG0 und IOCFG2 entsprechend berücksichtigt.

Als Sendemodul kommt ein CC1101 auf einem günstigen Breakout-Board aus China zum Einsatz.

Entwicklungsumgebung: Atmel Studio 7.0

Der Branch "dev" ist mein aktueller Entwicklungszweig - die Version kann evtl. fehlerhaft oder gar nicht compilierbar sein.
Im Zweifelsfall bitte die Releases verwenden.

Das Release 2.0 ist das letzte stabile und basiert auf der neuen C++ Lib von Trilu (vielen Dank dafür!)
