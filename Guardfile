$: << ENV['CRAILS_SHARED_DIR']
$: << "#{Dir.pwd}/scripts"
$: << "#{Dir.pwd}"

group :before_compile do
  guard 'metarecord',
        input: ['app/data'],
        output: 'lib',
        generators: ['crails/destroy','crails/query','crails/view','comet/edit','comet/data','archive'] do
    watch(%r{app/data/.+.rb$})
  end

  guard 'crails-odb', backends: [:pgsql],
    output:   "lib/odb",
    at_once:  true do
    watch(%r{app/models/.+.h(pp|xx)?$})
  end

  guard 'comet-html', source: 'app/comet', output: 'lib/comet/html', config: 'config/comet.json' do
    watch(%r{app/comet/.+\.html$})
  end
end

group :compile do
  guard 'comet', cmakelists: 'app/comet', output: 'public/assets'

  guard 'sass', input: 'app/assets/stylesheets', output: 'public/assets'

  guard 'crails-cmake' do
    watch('CMakeLists.txt')
    watch('build/CMakeCache.txt')
    watch(%r{^config/.+\.[hc](pp|xx)?$})
    watch(%r{^app/.+\.[hc](pp|xx)?$})
    watch(%r{^lib/.+\.[hc](pp|xx)?$})
    watch(%r{^tasks/.+\.[hc](pp|xx)?$})
    watch(%r{^spec/.+\.[hc](pp|xx)?$})
  end
end

group :tests do
  guard 'crails-tests' do
    watch(%r{^build/tests$})
  end
end
