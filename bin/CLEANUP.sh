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

# Avoid bashing the github API multiple times
# Store the output in a temp file so we can
# make multiple passes
GH_REPOS_TMP=$(mktemp)
GH_FORKS_TMP=$(mktemp -d)
GH_AUTH_TMP=$(mktemp)

# Misc vars
FORK_DIR_NAME=FORKS

# Runtime vars
FORKS_DIR="${ROOT_DIR}/${FORK_DIR_NAME}"

function cleanup() {
  echo
  echo "Cleaning up temporary files"
  rm -rf ${GH_REPOS_TMP} ${GH_FORKS_TMP}

  echo "Ensuring authentication scratch pad is removed"
  if [ -f "${GH_AUTH_TMP}" ]; then
    rm -rf ${GH_AUTH_TMP}
  fi
}

# Define a trap to remove this temp file on
# exit.
trap cleanup EXIT

function authenticate() {
  echo -n "Github Username: "
  read GH_USERNAME

  if [ "${GH_USERNAME}" == "" ]; then
    echo "Can not continue authentication with no username..."
    exit 1
  fi

  echo -n "Github Password: "
  read -s GH_PASSWORD
  echo

  if [ "${GH_PASSWORD}" == "" ]; then
    echo "Can not continue authentication with no password..."
    exit
  fi

  response=$(
    curl \
      --silent \
      --write-out %{http_code} \
      -X POST \
      -u $GH_USERNAME:$GH_PASSWORD \
      -H 'Content-Type: application/json' \
      -d '{"scopes": ["user:email"],"note": "Fork Consolidator"}' \
      https://api.github.com/authorizations -o "${GH_AUTH_TMP}"
  )

  case $response in
    401)
      msg=$(cat $GH_AUTH_TMP | jq -r '.message')
      echo "ERROR: ${msg}"
      exit 1
      break
  esac
}

# Fetch my public repos
function fetch_repos() {
  # type=source includes forks for some reason. I guess this only works for orgs
  response=$(
    curl \
      --silent \
      --write-out %{http_code} \
      -X GET \
      -u $GH_AUTH_TOKEN:x-oauth-basic \
      https://api.github.com/users/${GH_USERNAME}/repos -o $GH_REPOS_TMP
  )

  case $response in
    200)
      return
      ;;
    401)
      msg=$(cat $GH_AUTH_TMP | jq -r '.message')
      echo "ERROR: ${msg}"
      exit 1
      ;;
    *)
      echo "An error occured!"
      cat $GH_REPOS_TMP | jq -C .
  esac
}

# Find repos that are forks and collapse them into this repo as submodules
function collapse_forks() {
  cd $ROOT_DIR

  # Create forks dir if it does not exist
  [ ! -d "${FORKS_DIR}" ] && mkdir -p ${FORKS_DIR}

  for REPO_NAME in `cat $GH_REPOS_TMP | jq -r -c '.[] | select(.fork == true) | .name'`; do
    obj=${GH_FORKS_TMP}/$REPO_NAME.json
    cat $GH_REPOS_TMP | jq -r -c --arg repo $REPO_NAME '.[] | select(.fork == true) | select(.name == $repo)' > ${obj}

    len=$(wc -l $obj)
    if [[ $len == 0 ]]; then
      exit
    fi

    FORK_LANG=$(cat $obj | jq -r '.language')
    REPO_URL=$(cat $obj | jq -r '.url')

    if [ "${FORK_LANG}" == "null" ]; then
      FORK_LANG="Misc"
    fi

    DIR_LANG="${FORKS_DIR}/${FORK_LANG}"
    DIR_REPO_TREE="${DIR_LANG}/${REPO_NAME}"

    # Create LANG dir if it doesn't exist
    [ ! -d "${DIR_LANG}" ] && mkdir -p ${DIR_LANG}

    # Create repo dir if it doesn't exist
    [ ! -d "${DIR_REPO_TREE}" ] && mkdir -p ${DIR_REPO_TREE}

    # Save the listing for debugging
    cp $obj ${DIR_REPO_TREE}/repo_listing.json

    # Fetch repo info which contains parent info
    # Be nice to the github API
    if [ ! -f "${DIR_REPO_TREE}/repo_detail.json" ]; then
      response=$(
        curl \
          --silent \
          --write-out %{http_code} \
          -X GET \
          -u $GH_AUTH_TOKEN:x-oauth-basic \
          $REPO_URL -o ${DIR_REPO_TREE}/repo_detail.json
      )

      case $response in
        200)
          continue
          ;;
        401)
          msg=$(cat $GH_AUTH_TMP | jq -r '.message')
          echo "ERROR: ${msg}"
          return 1
          ;;
        *)
          echo "An error occured!"
          cat $GH_REPOS_TMP | jq -C .
          return 1
      esac
    fi
  done

  # Fill empty directories with .gitkeep files to avoid ugly web listings
  find ${FORKS_DIR} -maxdepth 1 -type d '!' -exec test -e "{}/.gitkeep" ';' -exec touch "{}/.gitkeep" ';'

  # Walk for any new additions
  for repo in `find ./${FORK_DIR_NAME} -mindepth 2 -maxdepth 2 -type d '!' -exec test -d '{}/tree/' ';' -print`; do
    REPO_NAME=$(basename $repo)
    SUBTREE_PATH=$(echo $repo | sed -e 's/^\.\///g')

    # Extract original parent clone url
    PARENT_CLONE_URL=$(cat ${repo}/repo_detail.json | jq -r '.parent.clone_url')

    # Add & Commit before merging the subtree
    $GIT add $repo/
    $GIT commit -m "[COLLAPSE] Add fork of ${REPO_NAME} to tree."
    echo "Running $GIT subtree add --prefix ${SUBTREE_PATH}/tree ${PARENT_CLONE_URL} master --squash"
    $GIT subtree add --prefix ${SUBTREE_PATH}/tree ${PARENT_CLONE_URL} master --squash
  done

  # $GIT add .
  # $GIT commit -m "[COLLAPSE] Add subtree forks"
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
  *)
    echo "Credentials are valid. Continuing"
    fetch_repos
    collapse_forks
esac
