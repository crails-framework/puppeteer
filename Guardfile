$: << ENV['CRAILS_SHARED_DIR']
$: << "#{Dir.pwd}/scripts"
$: << "#{Dir.pwd}"

group :before_compile do
  guard 'models', input: ["app/data"],
                  output: "lib",
                  generators: [:edit, :data, :view, :destroy, :query, :archive] do
    watch(%r{app/data/.+\.rb$})
  end

  guard 'models', input: ["app/data"],
                  output: "front",
                  generators: [:front] do
    watch(%r{app/data/.+\.rb$})
  end

  guard 'crails-archive'

  guard 'crails-cheerp', include_paths: ["/usr/local/include","."],
#   sourcemap_output: "public/assets/application.js.map",
#   sourcemap_standalone: true,
#   preexecute: true,
    dummy_var: 42 do
    watch(%r{front/.+\.(cpp|hpp)$})
  end

  guard 'crails-odb', backends: [:pgsql],
    requires: ["modules/odb/traits.hpp"],
    output:   "lib/odb",
    at_once:  true do
    watch(%r{app/models/.+.h(pp|xx)?$})
  end
end

group :compile do
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
