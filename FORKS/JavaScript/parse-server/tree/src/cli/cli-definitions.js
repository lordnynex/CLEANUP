function numberParser(key) {
  return function(opt) {
    opt = parseInt(opt);
    if (!Number.isInteger(opt)) {
      throw new Error(`The ${key} is invalid`);
    }
    return opt;
  }
}

function objectParser(opt) {
  if (typeof opt == 'object') {
    return opt;
  }
  return JSON.parse(opt)
}

function moduleOrObjectParser(opt) {
  if (typeof opt == 'object')  {
    return opt;
  }
  try {
    return JSON.parse(opt);
  } catch(e) {}
  return opt;
}

function booleanParser(opt) {
  if (opt == true || opt == "true" || opt == "1") {
    return true;
  }
  return false;
}

export default {
  "appId": {
    env: "PARSE_SERVER_APPLICATION_ID",
    help: "Your Parse Application ID",
    required: true
  },
  "masterKey": {
    env: "PARSE_SERVER_MASTER_KEY",
    help: "Your Parse Master Key",
    required: true
  },
  "port": {
     env: "PORT",
     help: "The port to run the ParseServer. defaults to 1337.",
     default: 1337,
     action: numberParser("port")
  },
  "databaseURI": {
    env: "PARSE_SERVER_DATABASE_URI",
    help: "The full URI to your mongodb database"
  },
  "databaseOptions": {
    env: "PARSE_SERVER_DATABASE_OPTIONS",
    help: "Options to pass to the mongodb client",
    action: objectParser
  },
  "collectionPrefix": {
    env: "PARSE_SERVER_COLLECTION_PREFIX",
    help: 'A collection prefix for the classes'
  },
  "serverURL": {
    env: "PARSE_SERVER_URL",
    help: "URL to your parse server with http:// or https://.",
  },
  "publicServerURL": {
    env: "PARSE_PUBLIC_SERVER_URL",
    help: "Public URL to your parse server with http:// or https://.",
  },
  "clientKey": {
    env: "PARSE_SERVER_CLIENT_KEY",
    help: "Key for iOS, MacOS, tvOS clients"
  },
  "javascriptKey": {
    env: "PARSE_SERVER_JAVASCRIPT_KEY",
    help: "Key for the Javascript SDK"
  },
  "restAPIKey": {
    env: "PARSE_SERVER_REST_API_KEY",
    help: "Key for REST calls"
  },
  "dotNetKey": {
    env: "PARSE_SERVER_DOT_NET_KEY",
    help: "Key for Unity and .Net SDK"
  },
  "cloud": {
    env: "PARSE_SERVER_CLOUD_CODE_MAIN",
    help: "Full path to your cloud code main.js"
  },
  "push": {
    env: "PARSE_SERVER_PUSH",
    help: "Configuration for push, as stringified JSON. See https://github.com/ParsePlatform/parse-server/wiki/Push",
    action: objectParser
  },
  "oauth": {
    env: "PARSE_SERVER_OAUTH_PROVIDERS",
    help: "Configuration for your oAuth providers, as stringified JSON. See https://github.com/ParsePlatform/parse-server/wiki/Parse-Server-Guide#oauth",
    action: objectParser
  },
  "fileKey": {
    env: "PARSE_SERVER_FILE_KEY",
    help: "Key for your files",
  },
  "facebookAppIds": {
    env: "PARSE_SERVER_FACEBOOK_APP_IDS",
    help: "Comma separated list for your facebook app Ids",
    type: "list",
    action: function(opt) {
      return opt.split(",")
    }
  },
  "enableAnonymousUsers": {
    env: "PARSE_SERVER_ENABLE_ANON_USERS",
    help: "Enable (or disable) anon users, defaults to true",
    action: booleanParser
  },
  "allowClientClassCreation": {
    env: "PARSE_SERVER_ALLOW_CLIENT_CLASS_CREATION",
    help: "Enable (or disable) client class creation, defaults to true",
    action: booleanParser
  },
  "mountPath": {
    env: "PARSE_SERVER_MOUNT_PATH",
    help: "Mount path for the server, defaults to /parse",
    default: "/parse"
  },
  "filesAdapter": {
    env: "PARSE_SERVER_FILES_ADAPTER",
    help: "Adapter module for the files sub-system",
    action: moduleOrObjectParser
  },
  "emailAdapter": {
    env: "PARSE_SERVER_EMAIL_ADAPTER",
    help: "Adapter module for the email sending",
    action: moduleOrObjectParser
  },
  "verifyUserEmails": {
    env: "PARSE_SERVER_VERIFY_USER_EMAILS",
    help: "Enable (or disable) user email validation, defaults to false",
    action: booleanParser
  },
  "appName": {
    env: "PARSE_SERVER_APP_NAME",
    help: "Sets the app name"
  },
  "loggerAdapter": {
    env: "PARSE_SERVER_LOGGER_ADAPTER",
    help: "Adapter module for the logging sub-system",
    action: moduleOrObjectParser
  },
  "liveQuery": {
    env: "PARSE_SERVER_LIVE_QUERY_OPTIONS",
    help: "liveQuery options",
    action: objectParser
  },
  "customPages": {
    env: "PARSE_SERVER_CUSTOM_PAGES",
    help: "custom pages for pasword validation and reset",
    action: objectParser
  },
  "maxUploadSize": {
    env: "PARSE_SERVER_MAX_UPLOAD_SIZE",
    help: "Max file size for uploads.",
    default: "20mb"
  },
  "sessionLength": {
    env: "PARSE_SERVER_SESSION_LENGTH",
    help: "Session duration, defaults to 1 year",
    action: numberParser("sessionLength")
  },
  "verbose": {
    env: "VERBOSE",
    help: "Set the logging to verbose"
  }
};
