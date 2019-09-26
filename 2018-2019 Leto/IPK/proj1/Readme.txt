* Popis řešného problému
Program ktorý prostredníctvom HTTP dotazu získa informácie o počasí 
z dátového zdroja OpenWatherMap. Vystupom sú aktuálne informácie o 
vybranom meste, konkrétne popis počasia, teplota, vlhkosť, tlak, 
rýchlosť vetra a smer vetra.

* Návrh řešení
Načítame vstup od užívateľa, vytvoríme socket, pripojíme sa na api, 
vytvoríme get request, odošleme do api, prijme odpoved, ziskane data prevedieme 
na json, vypíšeme konkrétne data.

* Instalace a překlad aplikace
Na beh programu je potrebné nainštalovať Python 3.
Pre prístup je nutná bezplatná registrácia a získanie api kľúča 
ktorý sa používa pri spúšťaní aplikácie. Registrujte sa 
tu: https://openweathermap.org.

* Jak spustit aplikaci?
make run api_key=<API klíč> city=<Město>
Pozn.: ak sa názov mesta skladá z viac slov dáme ho do 
úvodzoviek napríklad "New York"