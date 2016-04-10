#ifndef __TESTS_AESECBSBOX128__
#define __TESTS_AESECBSBOX128__

#include <string>
#include <vector>

// Test vectors from <http://csrc.nist.gov/groups/STM/cavp/documents/aes/KAT_AES.zip>

static const std::vector<std::string> AES128_ECB_SBOX_KEY = {
    "10a58869d74be5a374cf867cfb473859",
    "caea65cdbb75e9169ecd22ebe6e54675",
    "a2e2fa9baf7d20822ca9f0542f764a41",
    "b6364ac4e1de1e285eaf144a2415f7a0",
    "64cf9c7abc50b888af65f49d521944b2",
    "47d6742eefcc0465dc96355e851b64d9",
    "3eb39790678c56bee34bbcdeccf6cdb5",
    "64110a924f0743d500ccadae72c13427",
    "18d8126516f8a12ab1a36d9f04d68e51",
    "f530357968578480b398a3c251cd1093",
    "da84367f325d42d601b4326964802e8e",
    "e37b1c6aa2846f6fdb413f238b089f23",
    "6c002b682483e0cabcc731c253be5674",
    "143ae8ed6555aba96110ab58893a8ae1",
    "b69418a85332240dc82492353956ae0c",
    "71b5c08a1993e1362e4d0ce9b22b78d5",
    "e234cdca2606b81f29408d5f6da21206",
    "13237c49074a3da078dc1d828bb78c6f",
    "3071a2a48fe6cbd04f1a129098e308f8",
    "90f42ec0f68385f2ffc5dfc03a654dce",
    "febd9a24d8b65c1c787d50a4ed3619a9",
};

static const std::string AES128_ECB_SBOX_PLAIN = "00000000000000000000000000000000";

static const std::vector<std::string> AES128_ECB_SBOX_CIPHER = {
    "6d251e6944b051e04eaa6fb4dbf78465",
    "6e29201190152df4ee058139def610bb",
    "c3b44b95d9d2f25670eee9a0de099fa3",
    "5d9b05578fc944b3cf1ccf0e746cd581",
    "f7efc89d5dba578104016ce5ad659c05",
    "0306194f666d183624aa230a8b264ae7",
    "858075d536d79ccee571f7d7204b1f67",
    "35870c6a57e9e92314bcb8087cde72ce",
    "6c68e9be5ec41e22c825b7c7affb4363",
    "f5df39990fc688f1b07224cc03e86cea",
    "bba071bcb470f8f6586e5d3add18bc66",
    "43c9f7e62f5d288bb27aa40ef8fe1ea8",
    "3580d19cff44f1014a7c966a69059de5",
    "806da864dd29d48deafbe764f8202aef",
    "a303d940ded8f0baff6f75414cac5243",
    "c2dabd117f8a3ecabfbb11d12194d9d0",
    "fff60a4740086b3b9c56195b98d91a7b",
    "8146a08e2357f0caa30ca8c94d1a0544",
    "4b98e06d356deb07ebb824e5713f7be3",
    "7a20a53d460fc9ce0423a7a0764c6cf2",
    "f4a70d8af877f9b02b4c40df57d45b17",
};

#endif // __TESTS_AESECBSBOX128__
