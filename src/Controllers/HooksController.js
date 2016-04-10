/** @flow weak */

import * as DatabaseAdapter from "../DatabaseAdapter";
import * as triggers from "../triggers";
import * as Parse from "parse/node";
import * as request from "request";

const DefaultHooksCollectionName = "_Hooks";

export class HooksController {
  _applicationId:string;
  _collectionPrefix:string;
  _collection;

  constructor(applicationId:string, collectionPrefix:string = '') {
    this._applicationId = applicationId;
    this._collectionPrefix = collectionPrefix;
  }

  load() {
    return this._getHooks().then(hooks => {
      hooks = hooks || [];
      hooks.forEach((hook) => {
        this.addHookToTriggers(hook);
      });
    });
  }

  getCollection() {
    if (this._collection) {
      return Promise.resolve(this._collection)
    }

    let database = DatabaseAdapter.getDatabaseConnection(this._applicationId, this._collectionPrefix);
    return database.adaptiveCollection(DefaultHooksCollectionName).then(collection => {
      this._collection = collection;
      return collection;
    });
  }

  getFunction(functionName) {
    return this._getHooks({ functionName: functionName }, 1).then(results => results[0]);
  }

  getFunctions() {
    return this._getHooks({ functionName: { $exists: true } });
  }

  getTrigger(className, triggerName) {
    return this._getHooks({ className: className, triggerName: triggerName }, 1).then(results => results[0]);
  }

  getTriggers() {
    return this._getHooks({ className: { $exists: true }, triggerName: { $exists: true } });
  }

  deleteFunction(functionName) {
    triggers.removeFunction(functionName, this._applicationId);
    return this._removeHooks({ functionName: functionName });
  }

  deleteTrigger(className, triggerName) {
    triggers.removeTrigger(triggerName, className, this._applicationId);
    return this._removeHooks({ className: className, triggerName: triggerName });
  }

  _getHooks(query, limit) {
    let options = limit ? { limit: limit } : undefined;
    return this.getCollection().then(collection => collection.find(query, options));
  }

  _removeHooks(query) {
    return this.getCollection().then(collection => {
      return collection.deleteMany(query);
    }).then(() => {
      return {};
    });
  }

  saveHook(hook) {
    var query;
    if (hook.functionName && hook.url) {
      query = { functionName: hook.functionName }
    } else if (hook.triggerName && hook.className && hook.url) {
      query = { className: hook.className, triggerName: hook.triggerName }
    } else {
      throw new Parse.Error(143, "invalid hook declaration");
    }
    return this.getCollection()
      .then(collection => collection.upsertOne(query, hook))
      .then(() => {
        return hook;
      });
  }

  addHookToTriggers(hook) {
    var wrappedFunction = wrapToHTTPRequest(hook);
    wrappedFunction.url = hook.url;
    if (hook.className) {
      triggers.addTrigger(hook.triggerName, hook.className, wrappedFunction, this._applicationId)
    } else {
      triggers.addFunction(hook.functionName, wrappedFunction, null, this._applicationId);
    }
  }

  addHook(hook) {
    this.addHookToTriggers(hook);
    return this.saveHook(hook);
  }

  createOrUpdateHook(aHook) {
    var hook;
    if (aHook && aHook.functionName && aHook.url) {
      hook = {};
      hook.functionName = aHook.functionName;
      hook.url = aHook.url;
    } else if (aHook && aHook.className && aHook.url && aHook.triggerName && triggers.Types[aHook.triggerName]) {
      hook = {};
      hook.className = aHook.className;
      hook.url = aHook.url;
      hook.triggerName = aHook.triggerName;

    } else {
      throw new Parse.Error(143, "invalid hook declaration");
    }

    return this.addHook(hook);
  };

  createHook(aHook) {
    if (aHook.functionName) {
      return this.getFunction(aHook.functionName).then((result) => {
        if (result) {
          throw new Parse.Error(143, `function name: ${aHook.functionName} already exits`);
        } else {
          return this.createOrUpdateHook(aHook);
        }
      });
    } else if (aHook.className && aHook.triggerName) {
      return this.getTrigger(aHook.className, aHook.triggerName).then((result) => {
        if (result) {
          throw new Parse.Error(143, `class ${aHook.className} already has trigger ${aHook.triggerName}`);
        }
        return this.createOrUpdateHook(aHook);
      });
    }

    throw new Parse.Error(143, "invalid hook declaration");
  };

  updateHook(aHook) {
    if (aHook.functionName) {
      return this.getFunction(aHook.functionName).then((result) => {
        if (result) {
          return this.createOrUpdateHook(aHook);
        }
        throw new Parse.Error(143, `no function named: ${aHook.functionName} is defined`);
      });
    } else if (aHook.className && aHook.triggerName) {
      return this.getTrigger(aHook.className, aHook.triggerName).then((result) => {
        if (result) {
          return this.createOrUpdateHook(aHook);
        }
        throw new Parse.Error(143, `class ${aHook.className} does not exist`);
      });
    }
    throw new Parse.Error(143, "invalid hook declaration");
  };
}

function wrapToHTTPRequest(hook) {
  return (req, res) => {
    let jsonBody = {};
    for (var i in req) {
      jsonBody[i] = req[i];
    }
    if (req.object) {
      jsonBody.object = req.object.toJSON();
      jsonBody.object.className = req.object.className;
    }
    if (req.original) {
      jsonBody.original = req.original.toJSON();
      jsonBody.original.className = req.original.className;
    }
    let jsonRequest = {
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(jsonBody)
    };

    request.post(hook.url, jsonRequest, function (err, httpResponse, body) {
      var result;
      if (body) {
        if (typeof body == "string") {
          try {
            body = JSON.parse(body);
          } catch (e) {
            err = { error: "Malformed response", code: -1 };
          }
        }
        if (!err) {
          result = body.success;
          err = body.error;
        }
      }
      if (err) {
        return res.error(err);
      } else {
        return res.success(result);
      }
    });
  }
}

export default HooksController;
