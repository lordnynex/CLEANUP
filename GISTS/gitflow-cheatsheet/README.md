## Initialize

gitflow | git
--------|-----
`git flow init` | `git init`
 | `git commit --allow-empty -m "Initial commit"`
 | `git checkout -b develop master`


## Connect to the remote repository

gitflow | git
--------|-----
_N/A_ | `git remote add origin git@github.com:MYACCOUNT/MYREPO`


## Features

### Create a feature branch

gitflow | git
--------|-----
`git flow feature start MYFEATURE` | `git checkout -b feature/MYFEATURE develop`


### Share a feature branch

gitflow | git
--------|-----
`git flow feature publish MYFEATURE` | `git checkout feature/MYFEATURE`
 | `git push origin feature/MYFEATURE`


### Get latest for a feature branch

gitflow | git
--------|-----
`git flow feature pull origin MYFEATURE` | `git checkout feature/MYFEATURE`
 | `git pull --rebase origin feature/MYFEATURE`


### Finalize a feature branch

gitflow | git
--------|-----
`git flow feature finish MYFEATURE` | `git checkout develop`
 | `git merge --no-ff feature/MYFEATURE`
 | `git branch -d feature/MYFEATURE`


### Push the merged feature branch

gitflow | git
--------|-----
_N/A_ | `git push origin develop`
 | `git push origin :feature/MYFEATURE` _(if pushed)_


## Releases

### Create a release branch

gitflow | git
--------|-----
`git flow release start 1.2.0` | `git checkout -b release/1.2.0 develop`


### Share a release branch

gitflow | git
--------|-----
`git flow release publish 1.2.0` | `git checkout release/1.2.0`
 | `git push origin release/1.2.0`


### Get latest for a release branch

gitflow | git
--------|-----
_N/A_ | `git checkout release/1.2.0`
 | `git pull --rebase origin release/1.2.0`


### Finalize a release branch

gitflow | git
--------|-----
`git flow release finish 1.2.0` | `git checkout master`
 | `git merge --no-ff release/1.2.0`
 | `git tag -a 1.2.0`
 | `git checkout develop`
 | `git merge --no-ff release/1.2.0`
 | `git branch -d release/1.2.0`


### Push the merged feature branch

gitflow | git
--------|-----
_N/A_ | `git push origin master`
 | `git push origin develop`
 | `git push origin --tags`
 | `git push origin :release/1.2.0` _(if pushed)_


## Hotfixes

### Create a hotfix branch

gitflow | git
--------|-----
`git flow hotfix start 1.2.1 [commit]` | `git checkout -b hotfix/1.2.1 [commit]`


### Finalize a hotfix branch

gitflow | git
--------|-----
`git flow hotfix finish 1.2.1` | `git checkout master`
 | `git merge --no-ff hotfix/1.2.1`
 | `git tag -a 1.2.1`
 | `git checkout develop`
 | `git merge --no-ff hotfix/1.2.1`
 | `git branch -d hotfix/1.2.1`


### Push the merged hotfix branch

gitflow | git
--------|-----
_N/A_ | `git push origin master`
 | `git push origin develop`
 | `git push origin --tags`
 | `git push origin :hotfix/1.2.1` _(if pushed)_



## References

 - http://nvie.com/posts/a-successful-git-branching-model/
 - https://help.github.com/articles/using-pull-requests#shared-repository-model
 - Personal experience
