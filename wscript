#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from os import path

NAME = 'serialkiller'

top='.'
out='build'

GEN_DIR='gen'

def options(ctx):
    ctx.load('compiler_c compiler_cxx')
    ctx.add_option('--with-tests', action='store_true', default=False, help='Build tests', dest='with_tests')
    ctx.add_option('--staticlib', action='store_true', default=True, help='If true, build static library')

def configure(ctx):
    ctx.load('compiler_c compiler_cxx')

    _configure_libs(ctx)

    ctx.env.CXXFLAGS = ['-std=c++11', '-g']
    ctx.env.CFLAGS = ['-g']
    ctx.env.LDFLAGS = ['-g']

def _configure_libs(ctx):
    envVars = import_vars(ctx)

    env = ctx.env

    #commonexceptions
    env.STLIB_commonexceptions      = ['commonexceptions']
    env.STLIBPATH_commonexceptions  = [path.abspath(envVars['COMMONEXCEPTIONS_LIB'])]
    env.INCLUDES_commonexceptions   = [path.abspath(envVars['COMMONEXCEPTIONS_INCLUDES'])]

    #catch
    env.INCLUDES_CATCH = [path.abspath(envVars['CATCH_INCLUDES'])]

def build(ctx):
    _build_dependencies(ctx)

    ctx(target="serialkiller"
        , features          = feature_cxxlib(ctx)
        , source            = find_files(ctx, 'src/**/*.cpp')
        , includes          = ['src']
        , export_includes   = ['src']
        , use               = ['commonexceptions', 'jsoncpp']
        , install_path      = 'lib'
    )

    _install_files(ctx)

def _build_dependencies(ctx):
    jsoncpp_dir = 'libs/jsoncpp'

    ctx(target              ="jsoncpp"
        , features          = feature_cxxlib(ctx)
        , source            = [path.join(jsoncpp_dir, 'jsoncpp.cpp')]
        , cxxflags          = ['-std=c++11']
        , includes          = [jsoncpp_dir]
        , export_includes   = [jsoncpp_dir]
        , install_path      = 'lib'
    )

def _install_files(ctx):
    ctx.install_files(
              path.join('include', NAME)
            , find_files(ctx, 'src/**/*.h', 'src/**/*.hpp')
            , cwd            = ctx.path.find_dir('src')
            , relative_trick = True)

    jsoncpp_dir = path.join('libs', 'jsoncpp')
    ctx.install_files(
              path.join('include')
            , find_files(ctx, path.join(jsoncpp_dir, '**', '*.h'), path.join(jsoncpp_dir, '**', '*.hpp'))
            , cwd            = ctx.path.find_dir(jsoncpp_dir)
            , relative_trick = True)

def install_local(ctx):
    from waflib import Options
    Options.commands = ['install'] + Options.commands
    Options.options.destdir = ctx.path.make_node([GEN_DIR,'dist']).abspath()

######################################## Helpers ############################################################

def find_files(ctx, *k):
    return ctx.path.ant_glob(k, src=True,dir=False)

def import_vars(ctx):
    #extract variables defined on shell file to current environment variables
    result = ctx.cmd_and_log('. ./dependencies.sh && env', shell=True, cwd=top)

    vars = {}
    for line in result.splitlines():
        pair = line.split('=')
        if len(pair) == 2:
            vars[pair[0]] = pair[1]

    return vars

def feature_cxxlib(ctx):
    return ['cxx', "cxxstlib" if ctx.options.staticlib else "cxxshlib"]
