#!/bin/bash

set -e

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

# Avoid bashing the github API multiple times
# Store the output in a temp file so we can
# make multiple passes
GH_REPOS_TMP=$(mktemp)
GH_FORKS_TMP=$(mktemp -d)

# Misc vars
FORK_DIR_NAME=FORKS

# Runtime vars
FORKS_DIR="${ROOT_DIR}/${FORK_DIR_NAME}"

# Define a trap to remove this temp file on
# exit.
trap "rm -rf ${GH_REPOS_TMP} ${GH_FORKS_TMP}" EXIT

# Fetch my public repos
function fetch_repos() {
  # type=source includes forks for some reason. I guess this only works for orgs
  $CURL -s https://api.github.com/users/${GH_USERNAME}/repos -o $GH_REPOS_TMP
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
      $CURL -s $REPO_URL -o ${DIR_REPO_TREE}/repo_detail.json
    fi
  done

  # Fill empty directories with .gitkeep files to avoid ugly web listings
  find ${FORKS_DIR} -maxdepth 1 -type d '!' -exec test -e "{}/.gitkeep" ';' -exec touch "{}/.gitkeep" ';'

  # Walk for any new additions
  for repo in `find ./${FORK_DIR_NAME} -mindepth 2 -maxdepth 2 -type d '!' -exec test -d '{}/tree/' ';' -print`; do
    # Extract original parent clone url
    PARENT_CLONE_URL=$(cat ${repo}/repo_detail.json | jq -r '.parent.clone_url')

    # Add & Commit before merging the subtree
    $GIT add $repo/
    $GIT commit -m '[COLLAPSE] Add fork to tree.'
    $GIT subtree add --prefix ${repo}/tree ${PARENT_CLONE_URL} master --squash
  done
}

fetch_repos
collapse_forks
