# CLEANUP

I have this really annoying habit of forking repositories I like or want to experiment with instead of simply staring them. After awhile I end up with repositories everywhere and it's bugged me for long enough.

## Why?

Several years ago I made the mistake of writing some code that depended on someones experimental library. All worked well until the next day I went to work and attempted to pickup where I left off and the owner of this lib's repo deleted it because he believed it was 'bad work'. For convoluted reasons, I did not have a copy of the lib anymore. Even if I had a copy of it somewhere, it made me realize the ephemeral nature of peoples personal projects. Something they see as a temporary experiment with a short shelf life can sometimes serve as inspiration or reference for unrelated projects for years to come. I use these forks almost daily. They serve as architectural inspiration, points of reference, or simply a way for me to freeze something in time.

My hope is is that I will be able to use these things more effectively. I have a tendency sometimes to recall I have a fork that solves a similar problem, but absolutely can not remember which fork it was. At least now they'll be consolidated and I can use code searching tools to help.

## Scripts

The scripts have stubs to support generating temporary auth tokens from user credentils. Unfortunately, MFA is a bit more complicated.

It is possible with OATH tools to generate google authenticator tokens if you know your secret key. Unfortunately, I use a hardware token for MFA so things are a bit more complicated.

I wanted very much to keep the scripts self contained, without the need to export or specify auth tokens, but for me there is just no way to accomplish that.

These scripts look for an environment variable named `GH_CLEANUP_TOKEN`. The tokens github scopes should include `repos:read` & `repos:delete`

### Dependencies

- JQ
- CURL
- GIT
- Github personal access token

#### CLEANUP.sh

This script uses a personal auth token to find forks and add the parent as a subtree module organized by `LANG/name`. There is currently some issues in the order of operations with adding json state files (repo_detail.json & repo_listing.json) and the actual subtree.

For now this script adds the json state files, commits them, and then finally adds the subtree. It is annoying to have multiple commits per fork, but this prevents subtree from bailing due to any uncommitted changes in the tree.

**NOTE** Currently the script adds the origin repo's `master` branch at the time the script was run. It does not currently detect if the origin repo's default branch is not named `master`. Further it does not detect the commit HEAD points to in your fork. You get the up to date copy whether you like it or not. You can manually manipulate the subtree after if this behavior is undesirable.

#### DELETE-FORKS.sh

This script isn't thoroughly tested because I only got one shot to test it. This will not speak to github to detect repositories. It finds subtree's in `FORKS` dir that **1)** Do have a subtree attached and **2)** Do NOT have a `.deleted` state file.

If the above criteria is met, it will naively fire off a DELETE to the github API for that fork and create the state file on success. Thats it. No other error or sanity checks occur. Use at your own risk!

## License

Code/Scripts that are written by me (most likely `bin/*`) is licensed under the WTFPL.

[![WTFPL](http://www.wtfpl.net/wp-content/uploads/2012/12/wtfpl-badge-4.png)](http://www.wtfpl.net/)


Code/Scripts that are not written by me (most likely `FORKS/*`) is the property of it's respective author(s) and licensed accordingly. Feel free to dig around the parent repo for actual license info as I do not track it. 
