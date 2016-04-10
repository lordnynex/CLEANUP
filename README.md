# es6-native-set
## Set collection as specified in ECMAScript6

Uses native C++ unordered_set container, so the memory is stored
outside of node and it is a lot faster than native JS implementations.

As of 2.0.0, requires node.js 0.12 or later. If you are running
node.js 0.10, stick with the 1.x.x.

### Usage

```javascript
var Set = require('es6-native-set');
```

### Installation

	$ npm install es6-native-set

#### API

Best is to refer to [specification](http://people.mozilla.org/~jorendorff/es6-draft.html#sec-set-objects). Still if you want quick look, follow examples:

```javascript
var Set = require('es6-native-set');

var set = new Set();

set.add('raz').add('dwa').add({});
set.size;                 // 3
set.has('raz');           // true
set.has('foo');           // false
set.add('foo');           // set
set.size                  // 4
set.has('foo');           // true
set.has('dwa');           // true
set.delete('dwa');        // true
set.size;                 // 3

set.forEach(function (value) {
  // 'raz', {}, 'foo' iterated
});

var iterator = set.values();

iterator.next(); // { done: false, value: 'raz' }
iterator.next(); // { done: false, value: {} }
iterator.next(); // { done: false, value: 'foo' }
iterator.next(); // { done: true, value: undefined }

set.clear(); // undefined
set.size; // 0
```

This package is made possible because of [Grokker](http://grokker.com/), one of the best places to work. If you are a JS developer looking for a new gig, send me an email at &#x5b;'chad', String.fromCharCode(64), 'grokker', String.fromCharCode(0x2e), 'com'&#x5d;.join('').
