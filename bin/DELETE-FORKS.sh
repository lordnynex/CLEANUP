#!/bin/bash

# set -e

# A hacky script to cleanup my github account
#  - brandon beveridge

# Find the jq binary.
JQ=$(command -v jq 2>/dev/null)

# Find the curl binary.
CURL=$(command -v curl 2>/dev/null)

# Find the git binary.
GIT=$(command -v git 2>/dev/null)

# Abs path
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Root dir
ROOT_DIR=$($GIT rev-parse --show-toplevel)

# Github username
GH_USERNAME=lordnynex
GH_PASSWORD=""
GH_AUTH_TOKEN=""
GH_MFA_TOKEN=""

# Misc vars
FORK_DIR_NAME=FORKS

# Runtime vars
FORKS_DIR="${ROOT_DIR}/${FORK_DIR_NAME}"

function delete_fork() {
  # https://developer.github.com/v3/repos/#delete-a-repository
  response=$(
    curl \
      --silent \
      --write-out %{http_code} \
      -X DELETE \
      -u $GH_AUTH_TOKEN:x-oauth-basic \
      "https://api.github.com/repos/${1}" -o /dev/null
  )

  case $response in
    204)
      printf "  [%s] %s\n" "$response" "Success"
      touch "${2}/.deleted"
      ;;
    *)
      printf "  [%s] %s\n" "$response" "Failure"
  esac
}

function delete_forks() {
  for fork in `find ${FORKS_DIR} -mindepth 2 -maxdepth 2 -type d \( -exec test -d '{}/tree/' \; -a '!' -exec test -f '{}/.deleted' \; \) -print`; do
    if [ -f "$fork/repo_listing.json" ]; then
      REPO_NAME=$(cat "$fork/repo_listing.json" | jq -r '.full_name')
      echo "Attempting to delete ${REPO_NAME}"
      delete_fork "$REPO_NAME" "$fork"
    else
      echo "Skipping $fork because it has no metadata..."
    fi
  done
}


if [ -z ${GH_CLEANUP_TOKEN+x} ]; then
  # authenticate
  echo "Not supporting manual auth right now because of MFA complexities..."
  exit 1
else
  echo "Bypassing authentication and using detected personal access token..."
  GH_AUTH_TOKEN=$GH_CLEANUP_TOKEN
fi

response=$(
  curl \
    --silent \
    --write-out %{http_code} \
    -X GET \
    -u $GH_AUTH_TOKEN:x-oauth-basic \
    https://api.github.com/user -o "${GH_AUTH_TMP}"
)

case $response in
  401)
    msg=$(cat $GH_AUTH_TMP | jq -r '.message')
    echo "ERROR: ${msg}"
    exit 1
    ;;
  200)
    echo "Credentials are valid. Continuing [$response]"
    echo
    delete_forks
    ;;
  *)
    echo "Something is wrong.."
    cat $GH_AUTH_TMP | jq . '.'
esac
