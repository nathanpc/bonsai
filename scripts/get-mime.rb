#!/usr/bin/env ruby

require 'scripts/mime-types/lib/mime/types.rb'

print MIME::Types.type_for(ARGV[0]).first.content_type
