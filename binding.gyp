{
  'targets': [
    {
      'conditions': [
        [ 'OS!="win"', {
          'cflags': [ '-std=c++11', '-W', '-Wall', '-pthread', '-g', '-fPIC' ],
          'link_settings': {
            'libraries': [ '-L.', '/usr/local/lib/libliveMedia.a', '/usr/local/lib/libgroupsock.a', '/usr/local/lib/libUsageEnvironment.a', '/usr/local/lib/libBasicUsageEnvironment.a' ],
          },
          'include_dirs' : [
            'inc',
            '/usr/local/include/liveMedia',
            '/usr/local/include/groupsock',
            '/usr/local/include/UsageEnvironment',
            '/usr/local/include/BasicUsageEnvironment/'
          ]
        }]
      ],
      'target_name': 'eas-rtsp',
      'sources': [
        'src/rtspconnectionclient.cpp', 
        'src/environment.cpp',
        'src/eas-rtsp.cc'
      ],
      'include_dirs' : [
        '<!(node -e "require(\'nan\')")',
        '<!(node -e "require(\'streaming-worker-sdk\')")'
      ],
    }
  ]
}