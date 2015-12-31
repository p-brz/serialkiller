# SerialKiller

(alpha) C++ library to help with (de)serialization of objects.

This library was created as part of the [CalmPhotoFrame](https://bitbucket.org/pairg-ufrn/calmphotoframe-core) project within the [PAIRG research group](http://www.pairg.dimap.ufrn.br) in [UFRN](http://www.ufrn.br) (Federal University of Rio Grande do Norte).
The code was extracted to allow the reuse by other projects.

## Features

The code is still "primitive" (i hope to change the api a lot), but it allows:

* (De)Serialize from/to JSON
* Easy to add new kinds of serializers (e.g. xml, yaml, ?)
* Implement Serializable and Deserializable interfaces on your entities and use it with any Serializer

## How to Build

### Dependencies

This project uses the [waf](https://waf.io/book/) build system.
So, it requires python to build the project, and some c++ compiler (tested on gcc).

The project also uses (*experimentally*) [npm](https://www.npmjs.com/) to handle the dependencies. So you will need to install node and npm.

You can also download the dependencies 'by hand' (see the package.json for them), but it is not recommended.

Change the *dependencies.sh* file to configure dependencies location (in future, i hope to automate this to use with npm).

### Commands

After installing the required dependencies

Run:

    #download dependencies
    npm install

    #configure and build project
    ./waf configure build

For help on commands and allowed options, see:

    ./waf --help
