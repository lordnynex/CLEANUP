package main

import (
  "bytes"
  "code.google.com/p/go.crypto/openpgp"
  "code.google.com/p/go.crypto/openpgp/armor"
  "fmt"
  "io/ioutil"
  "log"
)

func main() {
  decbuf := bytes.NewBuffer([]byte(encryptedMessage))
  result, err := armor.Decode(decbuf)
  if err != nil {
    log.Fatal(err)
  }

  md, err := openpgp.ReadMessage(result.Body, nil, func(keys []openpgp.Key, symmetric bool) ([]byte, error) {
    return []byte("golang"), nil
  }, nil)
  if err != nil {
    log.Fatal(err)
  }

  fmt.Println("dec version:", result.Header["Version"])
  fmt.Println("dec type:", result.Type)
  bytes, err := ioutil.ReadAll(md.UnverifiedBody)
  fmt.Println("md:", string(bytes))
}

const encryptedMessage = `-----BEGIN PGP MESSAGE-----
Version: GnuPG v1.4.15 (Darwin)

jA0EAwMCSk50dj2NcPtgySLEBzaZ+zgxODr+/7BeQPHyW4JsOrYXptQKFgQtewBg
HBi7
=dz85
-----END PGP MESSAGE-----`
