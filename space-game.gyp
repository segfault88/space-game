{
  'targets': [
    {
      'target_name': 'space-game',
      'type': 'executable',
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
            '/usr/local/Cellar/sdl2/2.0.3/include/SDL2/',
            '/usr/local/Cellar/glew/1.10.0/include/',
            '/usr/local/Cellar/glm/0.9.5.4/include/',
            '/usr/local/Cellar/freeimage/3.16.0/include/'
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
