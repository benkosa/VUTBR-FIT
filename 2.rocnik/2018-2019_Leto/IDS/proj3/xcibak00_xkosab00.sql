drop table koncert;
drop table vstupenka;
drop table uzivatel;
drop table interpret;
drop table stage;

drop table festival;
drop table album;
drop table piesen;
drop table clen;
drop table zakaznik;
drop table cennik;

create table festival(
    id_festival integer GENERATED BY DEFAULT AS IDENTITY (START WITH 1  INCREMENT BY 1) NOT NULL,
    nazov varchar2(50) NOT NULL,
    miesto_konania varchar2(50) NOT NULL,
    obdobie_konania_od date NOT NULL,
    obdobie_konania_do date NOT NULL,
    kapacita_miest integer NOT NULL,
    kapacita_interpretov integer NOT NULL
);

-- chybajuca tabulka pre rocnik (verzia festivalu, ktora ma atribut navyse pre rocnik konania)

create table stage(
    id_stage integer GENERATED BY DEFAULT AS IDENTITY (START WITH 1  INCREMENT BY 1) NOT NULL,
    nazov varchar2(50) NOT NULL,
    plocha integer NOT NULL,
    id_festival integer NOT NULL
); 

create table album (
    id_album integer GENERATED BY DEFAULT AS IDENTITY (START WITH 1  INCREMENT BY 1) NOT NULL,
    nazov varchar2(50) NOT NULL,
    datum_vydania date NOT NULL
);

create table interpret (
    id_interpret integer GENERATED BY DEFAULT AS IDENTITY (START WITH 1  INCREMENT BY 1) NOT NULL,
    nazov varchar2(50) NOT NULL,
    zaner varchar2(50) NOT NULL,
    datum_vzniku date NOT NULL,
    id_stage integer NOT NULL,
    id_clen integer NOT NULL,
    id_piesen integer NOT NULL,
    id_album integer NOT NULL   
);


create table piesen (
    id_piesen integer GENERATED BY DEFAULT AS IDENTITY (START WITH 1  INCREMENT BY 1) NOT NULL,
    nazov varchar2(50) NOT NULL,
    dlzka_trvania timestamp NOT NULL,
    id_album integer NOT NULL
);

create table clen (
    id_clen integer GENERATED BY DEFAULT AS IDENTITY (START WITH 1  INCREMENT BY 1) NOT NULL,
    meno varchar2(50) NOT NULL,
    priezvisko varchar2(50) NOT NULL
);

create table uzivatel (
    id_uzivatel integer GENERATED BY DEFAULT AS IDENTITY (START WITH 1  INCREMENT BY 1) NOT NULL,
    prezyvka varchar2(50) NOT NULL, -- mohla by byt namiesto id - nemozu mat dvaja rovnaku prezyvku (login), v ER sme to vsak modelovali takto
    opravnenie varchar2(20) check (opravnenie in ('zaklad', 'admin', 'spravca')),
    id_interpret integer NOT NULL,
    id_zakaznik integer NOT NULL
);

create table koncert (
    id_koncert integer GENERATED BY DEFAULT AS IDENTITY (START WITH 1  INCREMENT BY 1) NOT NULL,
    nazov varchar2(50) NOT NULL,
    mesto varchar2(50) NOT NULL,
    datum_a_cas_konania date NOT NULL,
    id_cennik integer NOT NULL
);

create table zakaznik ( -- uvazuje zakaznikov z jednej krajiny
    id_zakaznik integer GENERATED BY DEFAULT AS IDENTITY (START WITH 1  INCREMENT BY 1) NOT NULL,
    meno varchar2(50) NOT NULL,
    priezvisko varchar2(50) NOT NULL,
    mesto varchar2(50) NOT NULL,
    ulica varchar2(50) NOT NULL,
    cislo_domu varchar2(50) NOT NULL,
    telefonne_cislo integer NOT NULL,
    email varchar2(50) NOT NULL
);

create table cennik (
    id_cennik integer GENERATED BY DEFAULT AS IDENTITY (START WITH 1  INCREMENT BY 1) NOT NULL,
    cena_statie integer NOT NULL,
    cena_sedenie integer NOT NULL,
    cena_vip integer NOT NULL
);

create table vstupenka (
    id_vstupenka integer GENERATED BY DEFAULT AS IDENTITY (START WITH 1  INCREMENT BY 1) NOT NULL,
    datum_a_cas_zakupenia date NOT NULL,
    typ varchar2(7) check (typ in ('statie', 'sedenie', 'VIP')),
    id_cennik integer NOT NULL
);

-- primarne kluce pre vsetky tabulky:
ALTER TABLE festival ADD CONSTRAINT pk_id_festival PRIMARY KEY(id_festival);
ALTER TABLE stage ADD CONSTRAINT pk_id_stage PRIMARY KEY(id_stage);
ALTER TABLE interpret ADD CONSTRAINT pk_id_interpret PRIMARY KEY(id_interpret);
ALTER TABLE album ADD CONSTRAINT pk_id_album PRIMARY KEY(id_album);
ALTER TABLE piesen ADD CONSTRAINT pk_id_piesen PRIMARY KEY(id_piesen);
ALTER TABLE clen ADD CONSTRAINT pk_id_clen PRIMARY KEY(id_clen);
ALTER TABLE uzivatel ADD CONSTRAINT pk_id_uzivatel PRIMARY KEY(id_uzivatel); -- primarnym klucom by mohla byt prezyvka, avsak v ER tak modelovana nie je
ALTER TABLE koncert ADD CONSTRAINT pk_id_koncert PRIMARY KEY(id_koncert);
ALTER TABLE zakaznik ADD CONSTRAINT pk_id_zakaznik PRIMARY KEY(id_zakaznik);
ALTER TABLE cennik ADD CONSTRAINT pk_id_cennik PRIMARY KEY(id_cennik);
ALTER TABLE vstupenka ADD CONSTRAINT pk_id_vstupenka PRIMARY KEY(id_vstupenka);

-- cuzdie kluce pre vsetky tabulky:
ALTER TABLE stage ADD CONSTRAINT fk_id_festival FOREIGN KEY (id_festival) REFERENCES festival(id_festival);

ALTER TABLE interpret ADD CONSTRAINT fk_id_stage FOREIGN KEY (id_stage) REFERENCES stage(id_stage);
ALTER TABLE interpret ADD CONSTRAINT fk_id_clen FOREIGN KEY (id_clen) REFERENCES clen(id_clen);
ALTER TABLE interpret ADD CONSTRAINT fk_id_piesen FOREIGN KEY (id_piesen) REFERENCES piesen(id_piesen);
ALTER TABLE interpret ADD CONSTRAINT fk_id_album FOREIGN KEY (id_album) REFERENCES album(id_album);

ALTER TABLE uzivatel ADD CONSTRAINT fk_id_interpret FOREIGN KEY (id_interpret) REFERENCES interpret(id_interpret);
ALTER TABLE uzivatel ADD CONSTRAINT fk_id_zakaznik FOREIGN KEY (id_zakaznik) REFERENCES zakaznik(id_zakaznik);

ALTER TABLE koncert ADD CONSTRAINT fk_id_cennik FOREIGN KEY (id_cennik) REFERENCES cennik(id_cennik);

ALTER TABLE vstupenka ADD CONSTRAINT fk_id_cennik2 FOREIGN KEY (id_cennik) REFERENCES cennik(id_cennik);

ALTER TABLE piesen ADD CONSTRAINT fk_id_album2 FOREIGN KEY (id_album) REFERENCES album(id_album);



-- plnenie tabuliek:
INSERT INTO festival(nazov, miesto_konania, obdobie_konania_od, obdobie_konania_do, kapacita_miest, kapacita_interpretov) 
VALUES('Grape', 'Letisko Piestany', TO_TIMESTAMP('28-07-2019', 'DD-MM-YYYY'),TO_TIMESTAMP('03-08-2019', 'DD-MM-YYYY'), 4000, 300);
INSERT INTO festival(nazov, miesto_konania, obdobie_konania_od, obdobie_konania_do, kapacita_miest, kapacita_interpretov) 
VALUES('Pohoda', 'Trencin', TO_TIMESTAMP('05-08-2019', 'DD-MM-YYYY'),TO_TIMESTAMP('10-08-2019', 'DD-MM-YYYY'), 5000, 300);
INSERT INTO festival(nazov, miesto_konania, obdobie_konania_od, obdobie_konania_do, kapacita_miest, kapacita_interpretov) 
VALUES('TopFest', 'Nove Mesto nad Vahom', TO_TIMESTAMP('15-08-2019', 'DD-MM-YYYY'),TO_TIMESTAMP('20-08-2019', 'DD-MM-YYYY'), 3000, 300);

INSERT INTO stage(nazov, plocha, id_festival) VALUES('0', 50, 1);
INSERT INTO stage(nazov, plocha, id_festival) VALUES('1', 100, 3);
INSERT INTO stage(nazov, plocha, id_festival) VALUES('2', 80, 2);
INSERT INTO stage(nazov, plocha, id_festival) VALUES('3', 20, 1);
INSERT INTO stage(nazov, plocha, id_festival) VALUES('4', 40, 3);
INSERT INTO stage(nazov, plocha, id_festival) VALUES('5', 60, 2);

INSERT INTO clen(meno, priezvisko) VALUES('Jozko', 'Super');
INSERT INTO clen(meno, priezvisko) VALUES('Benjamni', 'Kosa');
INSERT INTO clen(meno, priezvisko) VALUES('Michal', 'Cibak');

INSERT INTO album (nazov, datum_vydania) VALUES('54 Dole Hlavou', TO_TIMESTAMP('01-01-2009', 'DD-MM-YYYY'));
INSERT INTO album (nazov, datum_vydania) VALUES('Innerspeaker', TO_TIMESTAMP('01-01-2010', 'DD-MM-YYYY'));
INSERT INTO album (nazov, datum_vydania) VALUES('Relaxer', TO_TIMESTAMP('01-01-2017', 'DD-MM-YYYY'));

INSERT INTO piesen (nazov, dlzka_trvania, id_album) VALUES('3WW',  TO_TIMESTAMP('05-00','MM-SS'), 1);
INSERT INTO piesen (nazov, dlzka_trvania, id_album) VALUES('World wide',  TO_TIMESTAMP('04-51','MM-SS'), 1);
INSERT INTO piesen (nazov, dlzka_trvania, id_album) VALUES('Music To Walk Home By', TO_TIMESTAMP('05-12','MM-SS'), 2);
INSERT INTO piesen (nazov, dlzka_trvania, id_album) VALUES('Mew Mew', TO_TIMESTAMP('06-10','MM-SS'), 2);
INSERT INTO piesen (nazov, dlzka_trvania, id_album) VALUES('Intro',  TO_TIMESTAMP('01-22','MM-SS'), 3);
INSERT INTO piesen (nazov, dlzka_trvania, id_album) VALUES('New World',  TO_TIMESTAMP('01-50','MM-SS'), 3);
INSERT INTO piesen (nazov, dlzka_trvania, id_album) VALUES('Universe',  TO_TIMESTAMP('05-01','MM-SS'), 3);

INSERT INTO interpret(nazov, zaner, datum_vzniku, id_stage, id_clen, id_piesen, id_album) VALUES('Tame Impala', 'Psychedelic Rock', TO_TIMESTAMP('12-12-2007', 'DD-MM-YYYY'),1,1,1,1);
INSERT INTO interpret(nazov, zaner, datum_vzniku, id_stage, id_clen, id_piesen, id_album) VALUES('Horkyze Slize', 'Rock', TO_TIMESTAMP('01-01-1992', 'DD-MM-YYYY'),2,2,2,2);
INSERT INTO interpret(nazov, zaner, datum_vzniku, id_stage, id_clen, id_piesen, id_album) VALUES('Alt-J', 'Indie', TO_TIMESTAMP('01-01-2007', 'DD-MM-YYYY'),3,3,3,3);

INSERT INTO zakaznik(meno, priezvisko, mesto, ulica, cislo_domu, telefonne_cislo, email) 
VALUES('Benajmin', 'Kosa', 'Martin', 'Valca', 55, 421918459164, 'benkosa0@gmail.com');
INSERT INTO zakaznik(meno, priezvisko, mesto, ulica, cislo_domu, telefonne_cislo, email) 
VALUES('Michak', 'Cibak', 'Turcianske Teplice', 'Diviaky', 69, 421911159666, 'xcibak00@gmail.com');

INSERT INTO uzivatel(prezyvka, opravnenie, id_interpret, id_zakaznik) VALUES('Benji', 'zaklad', 1, 1);
INSERT INTO uzivatel(prezyvka, opravnenie, id_interpret, id_zakaznik) VALUES('Michal', 'admin', 2, 1);
INSERT INTO uzivatel(prezyvka, opravnenie, id_interpret, id_zakaznik) VALUES('Fero', 'spravca', 2, 1);

INSERT INTO cennik(cena_statie, cena_sedenie, cena_vip) VALUES(300, 500, 1000);
INSERT INTO cennik(cena_statie, cena_sedenie, cena_vip) VALUES(400, 700, 1200);
INSERT INTO cennik(cena_statie, cena_sedenie, cena_vip) VALUES(30, 50, 100);

INSERT INTO koncert(nazov, mesto, datum_a_cas_konania, id_cennik) VALUES('Intro', 'Martin', TO_TIMESTAMP('10-00-01-07-2017', 'HH24-Mi-DD-MM-YYYY'), 1);
INSERT INTO koncert(nazov, mesto, datum_a_cas_konania, id_cennik) VALUES('Alfa', 'Brno', TO_TIMESTAMP('10-00-01-07-2017', 'HH24-Mi-DD-MM-YYYY'), 2);
INSERT INTO koncert(nazov, mesto, datum_a_cas_konania, id_cennik) VALUES('Beta', 'Valca', TO_TIMESTAMP('10-00-01-07-2017', 'HH24-Mi-DD-MM-YYYY'), 3);

INSERT INTO vstupenka(datum_a_cas_zakupenia, typ, id_cennik) VALUES(TO_TIMESTAMP('13-50-01-01-2017', 'HH24-Mi-DD-MM-YYYY'), 'sedenie', 1);
INSERT INTO vstupenka(datum_a_cas_zakupenia, typ, id_cennik) VALUES(TO_TIMESTAMP('06-45-01-01-2017', 'HH24-Mi-DD-MM-YYYY'), 'statie', 2);
INSERT INTO vstupenka(datum_a_cas_zakupenia, typ, id_cennik) VALUES(TO_TIMESTAMP('03-15-01-01-2017', 'HH24-Mi-DD-MM-YYYY'), 'VIP', 3);

-- dotazy nad tabulkami:
    -- Ktor� albumy obsahuj� piese� dlh�iu ako 5 min�t?
SELECT DISTINCT album.nazov, album.datum_vydania
FROM album, piesen
WHERE album.id_album=piesen.id_album AND piesen.dlzka_trvania>TO_TIMESTAMP('05-00','MM-SS');

SELECT koncert.nazov, cennik.CENA_SEDENIE FROM koncert NATURAL JOIN cennik --na ktorom koncerte je cena za sedenie > 50
where cennik.CENA_SEDENIE > 50;

SELECT koncert.nazov, cennik.CENA_STATIE FROM koncert NATURAL JOIN cennik --na ktorom koncerte je cena za statie > 300
where cennik.CENA_STATIE > 300;

SELECT koncert.nazov, cennik.CENA_VIP FROM koncert NATURAL JOIN cennik --na ktorom koncerte je cena za vip <= 1000
where cennik.CENA_VIP <= 1000;
