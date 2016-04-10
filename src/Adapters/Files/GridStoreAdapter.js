/**
 GridStoreAdapter
 Stores files in Mongo using GridStore
 Requires the database adapter to be based on mongoclient

 @flow weak
 */

import { MongoClient, GridStore, Db} from 'mongodb';
import { FilesAdapter } from './FilesAdapter';

export class GridStoreAdapter extends FilesAdapter {
  _databaseURI: string;
  _connectionPromise: Promise<Db>;

  constructor(mongoDatabaseURI: string) {
    super();
    this._databaseURI = mongoDatabaseURI;
    this._connect();
  }

  _connect() {
    if (!this._connectionPromise) {
      this._connectionPromise = MongoClient.connect(this._databaseURI);
    }
    return this._connectionPromise;
  }

  // For a given config object, filename, and data, store a file
  // Returns a promise
  createFile(filename: string, data, contentType) {
    return this._connect().then(database => {
      let gridStore = new GridStore(database, filename, 'w');
      return gridStore.open();
    }).then(gridStore => {
      return gridStore.write(data);
    }).then(gridStore => {
      return gridStore.close();
    });
  }

  deleteFile(filename: string) {
    return this._connect().then(database => {
      let gridStore = new GridStore(database, filename, 'w');
      return gridStore.open();
    }).then((gridStore) => {
      return gridStore.unlink();
    }).then((gridStore) => {
      return gridStore.close();
    });
  }

  getFileData(filename: string) {
    return this._connect().then(database => {
      return GridStore.exist(database, filename)
        .then(() => {
          let gridStore = new GridStore(database, filename, 'r');
          return gridStore.open();
        });
    }).then(gridStore => {
      return gridStore.read();
    });
  }

  getFileLocation(config, filename) {
    return (config.mount + '/files/' + config.applicationId + '/' + encodeURIComponent(filename));
  }
}

export default GridStoreAdapter;
