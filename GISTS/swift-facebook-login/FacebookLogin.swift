//If you have a Bridging-Header:
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>

//In your AppDelegate:

func application(application: UIApplication, didFinishLaunchingWithOptions launchOptions: [String: AnyObject]?) -> Bool {
    //App launch code
    FBSDKApplicationDelegate.sharedInstance().application(application, didFinishLaunchingWithOptions: launchOptions)
    //Optionally add to ensure your credentials are valid:
    FBSDKLoginManager.renewSystemCredentials { (result:ACAccountCredentialRenewResult, error:NSError!) -> Void in
        //
    }
}

func application(application: UIApplication, openURL url: NSURL, sourceApplication: String?, annotation: AnyObject?) -> Bool {
    //Even though the Facebook SDK can make this determinitaion on its own,
    //let's make sure that the facebook SDK only sees urls intended for it,
    //facebook has enough info already!
    let isFacebookURL = url.scheme != nil && url.scheme!.hasPrefix("fb\(FBSDKSettings.appID())") && url.host == "authorize"
    if isFacebookURL {
        return FBSDKApplicationDelegate.sharedInstance().application(application, openURL: url, sourceApplication: sourceApplication, annotation: annotation)
    }
    return false
}

func applicationDidBecomeActive(application: UIApplication) {
    //App activation code
    FBSDKAppEvents.activateApp()
}

//Here's the facebook login code, have your login procedure call this:

let facebookReadPermissions = ["public_profile", "email", "user_friends"]
//Some other options: "user_about_me", "user_birthday", "user_hometown", "user_likes", "user_interests", "user_photos", "friends_photos", "friends_hometown", "friends_location", "friends_education_history"

func loginToFacebookWithSuccess(successBlock: () -> (), andFailure failureBlock: (NSError?) -> ()) {

    if FBSDKAccessToken.currentAccessToken() != nil {
        //For debugging, when we want to ensure that facebook login always happens
        //FBSDKLoginManager().logOut()
        //Otherwise do:
        return
    }

    FBSDKLoginManager().logInWithReadPermissions(self.facebookReadPermissions, handler: { (result:FBSDKLoginManagerLoginResult!, error:NSError!) -> Void in
        if error != nil {
            //According to Facebook:
            //Errors will rarely occur in the typical login flow because the login dialog
            //presented by Facebook via single sign on will guide the users to resolve any errors.

            // Process error
            FBSDKLoginManager().logOut()
            failureBlock(error)
        } else if result.isCancelled {
            // Handle cancellations
            FBSDKLoginManager().logOut()
            failureBlock(nil)
        } else {
            // If you ask for multiple permissions at once, you
            // should check if specific permissions missing
            var allPermsGranted = true

            //result.grantedPermissions returns an array of _NSCFString pointers
            let grantedPermissions = result.grantedPermissions.allObjects.map( {"\($0)"} )
            for permission in self.facebookReadPermissions {
                if !contains(grantedPermissions, permission) {
                    allPermsGranted = false
                    break
                }
            }
            if allPermsGranted {
                // Do work
                let fbToken = result.token.tokenString
                let fbUserID = resutl.token.userID

                //Send fbToken and fbUserID to your web API for processing, or just hang on to that locally if needed
                //self.post("myserver/myendpoint", parameters: ["token": fbToken, "userID": fbUserId]) {(error: NSError?) ->() in
                //	if error != nil {
                //		failureBlock(error)
                //	} else {
                //		successBlock(maybeSomeInfoHere?)
                //	}
                //}

                successBlock()
            } else {
                //The user did not grant all permissions requested
                //Discover which permissions are granted
                //and if you can live without the declined ones

                failureBlock((nil)
            }
        }
    })
}
