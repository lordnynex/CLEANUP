package main

// https://godoc.org/github.com/inconshreveable/go-update

// This gist documents go-install using a SHA-256 checksum,
// elliptic curve (prime 256) encrypted signature & bsdiff
// formatted patch.

// These steps took me over a hour to figure. go-install currently
// doesn't include a tutorial or quick start guide, so I created this
// gist just incase someone else might have to go through the same 
// process.

// Follow the steps below to witness a secure patch.

// Generate keys
//  * openssl ecparam -genkey -name prime256v1 -noout -out privateKey
//  * openssl ec -in privateKey -pubout -out publicKey
// Create checksum
//  * shasum -a 256 newexe | awk '{print $1}' > patch.hash
// Create diff patch
//  * bsdiff oldexe newexe > patch.diff
// Create signature
//  * openssl dgst -sha256 -sign privateKey newexe > newexe.signature
// Execute this program in the same directory
//  * go run main.go

import (
  "encoding/hex"
  "github.com/inconshreveable/go-update"
  "io/ioutil"
  "os"
  "strings"
)

func main() {
  file, err := os.Open("patch.diff")
  if err != nil {
    panic(err)
  }

  // Signature of the new executable, signed by the private cert
  signature, err := ioutil.ReadFile("newexe.signature")
  if err != nil {
    panic(err)
  }

  // SHA-256 hash of the patch file
  hash, err := ioutil.ReadFile("patch.hash")
  if err != nil {
    panic(err)
  }

  // Remove newline from the file
  checksum, err := hex.DecodeString(strings.TrimSpace(string(hash)))
  if err != nil {
    panic(err)
  }

  publicKey, err := ioutil.ReadFile("publicKey")
  if err != nil {
    panic(err)
  }

  opts := update.Options{
    TargetPath: "oldexe",
    Patcher:    update.NewBSDiffPatcher(),
    Checksum:   checksum,
    Signature:  signature,
  }

  err = opts.SetPublicKeyPEM(publicKey)
  if err != nil {
    panic(err)
  }

  err = update.Apply(file, opts)
  if err != nil {
    panic(err)
  }
}
