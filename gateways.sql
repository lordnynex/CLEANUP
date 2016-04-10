--
-- PostgreSQL database dump
--

SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

SET search_path = public, pg_catalog;

--
-- Data for Name: gateways; Type: TABLE DATA; Schema: public; Owner: biin
--

INSERT INTO gateways VALUES ('mt_infobip', 'mt_infobip', 'mt_infobip', 43038, 59, 0, 'INVALID', '');
INSERT INTO gateways VALUES ('mt_smsonline', 'mt_smsonline', 'mt_smsonline', 43038, 70, 0, 'INVALID', '');
INSERT INTO gateways VALUES ('mt_null', 'mt_null', 'mt_null', 43038, 99, 1, 'INVALID', 'Tariff=/home/biin/smsgate/null.xml');
INSERT INTO gateways VALUES ('mt_smst_ru', 'mt_smst_ru', 'mt_smst_ru', 43038, 99, 1, 'COUNTRY IN ["ru";]', 'Tariff=sms_smstraffic');
INSERT INTO gateways VALUES ('mt_smst_fr', 'mt_smst_fr', 'mt_smst_fr', 43038, 99, 1, 'COUNTRY IN ["ua";"kz";"ab";]', 'Tariff=sms_smstraffic');
INSERT INTO gateways VALUES ('mt_beepsend', 'mt_beepsend', 'mt_beepsend', 43038, 90, 1, 'NOT COUNTRY IN ["tm";"ru";"ua";"kz";"ab";]', 'Tariff=sms_beepsend');


--
-- PostgreSQL database dump complete
--

