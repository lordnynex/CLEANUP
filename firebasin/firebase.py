from .dataref import RootDataRef
try:
    from urllib import parse as urlparse
except ImportError:
    import urlparse


def Firebase(firebaseUrl):
    '''Construct a new Firebase reference from a full Firebase URL.'''

    url = urlparse.urlparse(firebaseUrl)
    root = RootDataRef('https://' + url.netloc)
    if url.path == '/' or url.path == '': 
        return root 
    else:
        return root.child(url.path[1:])
