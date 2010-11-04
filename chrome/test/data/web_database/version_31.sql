PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE meta(key LONGVARCHAR NOT NULL UNIQUE PRIMARY KEY,value LONGVARCHAR);
INSERT INTO "meta" VALUES('version','31');
INSERT INTO "meta" VALUES('last_compatible_version','31');
INSERT INTO "meta" VALUES('Default Search Provider ID','2');
INSERT INTO "meta" VALUES('Builtin Keyword Version','32');
CREATE TABLE keywords (id INTEGER PRIMARY KEY,short_name VARCHAR NOT NULL,keyword VARCHAR NOT NULL,favicon_url VARCHAR NOT NULL,url VARCHAR NOT NULL,show_in_default_list INTEGER,safe_for_autoreplace INTEGER,originating_url VARCHAR,date_created INTEGER DEFAULT 0,usage_count INTEGER DEFAULT 0,input_encodings VARCHAR,suggest_url VARCHAR,prepopulate_id INTEGER DEFAULT 0,autogenerate_keyword INTEGER DEFAULT 0,logo_id INTEGER DEFAULT 0,created_by_policy INTEGER DEFAULT 0,instant_url VARCHAR);
INSERT INTO "keywords" VALUES(2,'Google','google.com','http://www.google.com/favicon.ico','{google:baseURL}search?{google:RLZ}{google:acceptedSuggestion}{google:originalQueryForSuggestion}sourceid=chrome&ie={inputEncoding}&q={searchTerms}',1,1,'',0,0,'UTF-8','{google:baseSuggestURL}search?client=chrome&hl={language}&q={searchTerms}',1,1,6247,0,'{google:baseURL}webhp?{google:RLZ}sourceid=chrome-instant&ie={inputEncoding}&ion=1{searchTerms}');
INSERT INTO "keywords" VALUES(3,'Yahoo!','yahoo.com','http://search.yahoo.com/favicon.ico','http://search.yahoo.com/search?ei={inputEncoding}&fr=crmas&p={searchTerms}',1,1,'',0,0,'UTF-8','http://ff.search.yahoo.com/gossip?output=fxjson&command={searchTerms}',2,0,6264,0,'');
INSERT INTO "keywords" VALUES(4,'Bing','bing.com','http://www.bing.com/s/wlflag.ico','http://www.bing.com/search?q={searchTerms}',1,1,'',0,0,'UTF-8','http://api.bing.com/osjson.aspx?query={searchTerms}&language={language}',3,0,6241,0,'');
CREATE TABLE logins (origin_url VARCHAR NOT NULL, action_url VARCHAR, username_element VARCHAR, username_value VARCHAR, password_element VARCHAR, password_value BLOB, submit_element VARCHAR, signon_realm VARCHAR NOT NULL,ssl_valid INTEGER NOT NULL,preferred INTEGER NOT NULL,date_created INTEGER NOT NULL,blacklisted_by_user INTEGER NOT NULL,scheme INTEGER NOT NULL,UNIQUE (origin_url, username_element, username_value, password_element, submit_element, signon_realm));
CREATE TABLE web_app_icons (url LONGVARCHAR,width int,height int,image BLOB, UNIQUE (url, width, height));
CREATE TABLE web_apps (url LONGVARCHAR UNIQUE,has_all_images INTEGER NOT NULL);
CREATE TABLE autofill (name VARCHAR, value VARCHAR, value_lower VARCHAR, pair_id INTEGER PRIMARY KEY, count INTEGER DEFAULT 1);
CREATE TABLE autofill_dates ( pair_id INTEGER DEFAULT 0, date_created INTEGER DEFAULT 0);
CREATE TABLE autofill_profiles ( label VARCHAR, unique_id INTEGER PRIMARY KEY, first_name VARCHAR, middle_name VARCHAR, last_name VARCHAR, email VARCHAR, company_name VARCHAR, address_line_1 VARCHAR, address_line_2 VARCHAR, city VARCHAR, state VARCHAR, zipcode VARCHAR, country VARCHAR, phone VARCHAR, fax VARCHAR, date_modified INTEGER NOT NULL DEFAULT 0, guid VARCHAR NOT NULL DEFAULT "");
INSERT INTO "autofill_profiles" VALUES('Elvis Presley, 1122 PBJ Lane',1,'Elvis','','Presley','elvis@elvis.com','Hip Shake Inc.','1122 PBJ Lane','Suite 1','Memphis','TN','38116','USA','9013323322','',1288642516,'A4FF32F6-EF3F-379A-87F2-40A8811182A7');
CREATE TABLE credit_cards ( label VARCHAR, unique_id INTEGER PRIMARY KEY, name_on_card VARCHAR, type VARCHAR, card_number VARCHAR, expiration_month INTEGER, expiration_year INTEGER, verification_code VARCHAR, billing_address VARCHAR, shipping_address VARCHAR, card_number_encrypted BLOB, verification_code_encrypted BLOB, date_modified INTEGER NOT NULL DEFAULT 0, guid VARCHAR NOT NULL DEFAULT "");
INSERT INTO "credit_cards" VALUES('',2,'Jim J Jones','','',1,2011,'','0','',X'7631309863E9F1F33C2BDBFBFC86708448BDD8B37A495B628C8459A60D0CCD1047E69F',X'',1288642516,'B77F749C-8B0E-44B2-D299-3C80A95E0ADD');
CREATE TABLE token_service (service VARCHAR PRIMARY KEY NOT NULL,encrypted_token BLOB);
CREATE INDEX logins_signon ON logins (signon_realm);
CREATE INDEX web_apps_url_index ON web_apps (url);
CREATE INDEX autofill_name ON autofill (name);
CREATE INDEX autofill_name_value_lower ON autofill (name, value_lower);
CREATE INDEX autofill_dates_pair_id ON autofill_dates (pair_id);
CREATE INDEX autofill_profiles_label_index ON autofill_profiles (label);
CREATE INDEX credit_cards_label_index ON credit_cards (label);
COMMIT;
