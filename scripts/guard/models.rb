require 'cppgen/data_generator'
require 'cppgen/edit_generator'
require 'cppgen/view_generator'
require 'cppgen/destroy_generator'
require 'cppgen/aurelia_generator'
require 'cppgen/import_generator'
require 'cppgen/query_generator'
require 'cppgen/front_generator'
require 'cppgen/archive_generator'

Dir["config/**/*.rb"].each do |file|
  require "#{Dir.pwd}/#{file}"
end

module ::Guard
  class Models < Plugin
    def initialize options = {}
      super
      @input      = options[:input]
      @output     = options[:output]
      @generators = options[:generators]
      @input      = [@input] if @input.class != Array
      @tmpdir     = ".tmp"
    end

    def run_all
      `rm -Rf #{@tmpdir}`
      GeneratorBase.prepare @input, @tmpdir
      @generators.each do |generator|
        const_name = generator.to_s.capitalize + "Generator"
        klass      = Kernel.const_get const_name
	GeneratorBase.use klass
      end
      update_files
    end

    def update_files
      Dir["#{@tmpdir}/**/*"].each do |tmp_file|
        next if File.directory? tmp_file
        new_path = "#{@output}/#{tmp_file[@tmpdir.size + 1..tmp_file.size]}"
        if (not File.exists?(new_path)) || File.read(new_path) != File.read(tmp_file)
          puts "[models] generated #{new_path}"
          `mkdir -p '#{File.dirname new_path}'`
          `cp '#{tmp_file}' '#{new_path}'`
        else
          puts "[models] no updates required for #{new_path}"
        end
      end

      @input.each do |input|
        Dir["#{@output}/#{input}/**/*"].each do |actual_file|
          next if File.directory? actual_file
          tmp_path = "#{@tmpdir}/#{@output[@output.size + 1..@output.size]}"
          if not File.exists?(tmp_path)
            puts "[models] removed #{actual_file}"
            `rm '#{actual_file}'`
          end
        end
      end
    end
  end
end
