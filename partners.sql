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
-- Data for Name: partners; Type: TABLE DATA; Schema: public; Owner: biin
--

COPY partners (pid, uname, pass, cname, manager, balance, credit, plimit, postplay, trial, priority, phone, contact, tariff) FROM stdin;
100	test	test	TEST Account	Биин Илья	0	0	300	t	t	0			winsov
101	space	hjkfdsa	First Partner	Биин Илья	0	0	60	t	f	0			winsov
102	labirint	42p9y58t	Second Partner	Биин Илья	0	0	60	t	f	0			winsov
1	god	3dfx15gh	Green SMS	Биин Илья	0	0	300	t	f	1			winsov
104	biin	a6sent	Third Partner	Биин Илья	0	0	1	t	f	0			winsov
2	ermakov	A3R+c9E5			0	0	0	t	t	0			winsov
105	ermine	sdg345rty	ermine		0	0	60	t	f	0			winsov
103	winsov	ajh2397qs	Third Partner	Биин Илья	0	0	60	t	f	0			winsov
106	winsov2	ajh2397qs	Winsov рассылки	Биин Илья	20	0	30	f	f	0			winsov
\.


--
-- PostgreSQL database dump complete
--

