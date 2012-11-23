#!/usr/bin/env ruby

require 'rubygems'
require 'mime/types'

print MIME::Types.type_for(ARGV[0]).first.content_type
