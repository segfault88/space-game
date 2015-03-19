{
  'targets': [
    {
      'target_name': 'space-game',
      'type': 'executable',
      'configurations': {
        'Debug': {
          'defines': [ 'DEBUG', '_DEBUG' ],
          'msvs_settings': {
            'VCCLCompilerTool': {
              'RuntimeLibrary': 1, 
            },
            'VCLinkerTool': {
              'LinkTimeCodeGeneration': 1,
              'OptimizeReferences': 2,
              'EnableCOMDATFolding': 2,
              'LinkIncremental': 1,
              'GenerateDebugInformation': 'true',
              'AdditionalLibraryDirectories': [
              ]
            }          
          },
          'xcode_settings': {
            'OTHER_LDFLAGS': [
            ]
          }
        },
        'Release': {
          'defines': [ 'NDEBUG' ],
          'msvs_settings': {
            'VCCLCompilerTool': {
              'RuntimeLibrary': 0,
              'Optimization': 3,
              'FavorSizeOrSpeed': 1,
              'InlineFunctionExpansion': 2,
              'WholeProgramOptimization': 'true',
              'OmitFramePointers': 'true',
              'EnableFunctionLevelLinking': 'true',
              'EnableIntrinsicFunctions': 'true'            
            },
            'VCLinkerTool': {
              'LinkTimeCodeGeneration': 1,
              'OptimizeReferences': 2,
              'EnableCOMDATFolding': 2,
              'LinkIncremental': 1,
              'AdditionalLibraryDirectories': [
              ]            
            }          
          },
          'xcode_settings': {
           'OTHER_LDFLAGS': [
            ]
          }
        }
      },
      'dependencies': [
      ],
      'defines': [
      ],
      'include_dirs': [
      ],
      'sources': [
        'src/main.cpp'
      ],
      'conditions': [
        ['OS=="linux"', {
          'libraries': [
            '-lGL',
            '-lGLU',
            '-lSDL2'
          ]
        }],
        ['OS=="win"', {
          'libraries': [
            '-lopengl32.lib',
            '-lglu32.lib',
            '-lsdl2.lib'
          ]
        }],
        ['OS=="mac"', {
          'cflags': [
            '-std=c++11'
          ],
          'include_dirs': [
            '/usr/local/include/SDL2',
            '/usr/local/include'
          ],
          'library_dirs': [
            '/usr/local/lib'
          ],
          'libraries': [
            '$(SDKROOT)/System/Library/Frameworks/OpenGL.framework',
            'libSDL2.a',
            'libGLEW.a',
            'libfreeimage.a',
            'libfreeimageplus.a'
          ]
        }]
      ]        
    }
  ]
}
