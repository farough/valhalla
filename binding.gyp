{
<<<<<<< HEAD
    'includes': [ 'common.gypi' ],
    'include_dirs': [
        'valhalla',
    ],
    'libraries': [
        '-lvalhalla', '-Lvalhalla'
    ],
    "targets": [{
        "target_name": "node_route",
        "sources": [ "./src/bindings/node/node_route.cpp" ],
        'xcode_settings': {
            'GCC_ENABLE_CPP_RTTI': 'YES',
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'MACOSX_DEPLOYMENT_TARGET':'10.8',
            'CLANG_CXX_LIBRARY': 'libc++',
            'CLANG_CXX_LANGUAGE_STANDARD':'c++14',
            'GCC_VERSION': 'com.apple.compilers.llvm.clang.1_0'
        }
    }]
}
=======
    "includes": [ "common.gypi" ],
    "targets": [{
        "include_dirs": [
            "valhalla",
        ],
        "libraries": [
            "-lvalhalla"
        ],
        "target_name": "node_route",
        "sources": [ "./src/bindings/node/node_route.cpp" ],
        "xcode_settings": {
            "GCC_ENABLE_CPP_RTTI": "YES",
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
            "MACOSX_DEPLOYMENT_TARGET": "10.8",
            "CLANG_CXX_LIBRARY": "libc++",
            "CLANG_CXX_LANGUAGE_STANDARD": "c++14",
            "GCC_VERSION": "com.apple.compilers.llvm.clang.1_0"
        }
    }]
}
>>>>>>> 8eb187ab39523948943593bd57ec6bf9d3c422b3
