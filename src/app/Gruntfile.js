'use strict'

module.exports = function(grunt){
    require('time-grunt')(grunt);
    require('jit-grunt')(grunt, {
        useminPrepare: 'grunt-usemin'
    });
    grunt.initConfig({

        sass: {
            dist: {
                files: {
                    'css/styles.css': 'css/styles.scss'
                }
            }
        },
        watch: {
            files: 'css/*.scss',
            tasks: ['sass']
        },
        browserSync: {
            dev: {
                bsFile: {
                    src: [
                        'css/*.css',
                        '*.html',
                        'js/*.js'
                    ]
                },
                options: {
                    watchTask: true,
                    server: {
                        baseDir: './'
                    }
                }
            }
        },
        copy:{
            html: {
                files: [{
                    expand: true,
                    dot: true,
                    cwd: './',
                    src: ['*.html'],
                    dest: 'dist'
                }]
            },
            fonts: {
                files: [{
                    expand: true,
                    dot: true,
                    cwd: 'node_modules/@fortawesome/fontawesome-free',
                    src: ['webfonts/*.woff'],
                    dest: 'dist'
                }]
            }
        },
        clean: {
            build: {
                src: ['dist/']
            }
        },
        imagemin: {
            dynamic: {
                files: [{
                    expand: true,
                    dot: true,
                    cwd: './',
                    src: ['img/*.{png,jpg,git}'],
                    dest: 'dist/'
                }]
            }
        },
        useminPrepare: {
            foo: {
                dest: 'dist',
                src: ['list_users.html','login.html','menu_admin.html', 'menu_user.html', 'register_user.html']
            },
            options: {
                flow: {
                    steps: {
                        css: ['cssmin'],
                        js:['uglify']
                    },
                    post: {
                        css: [{
                            name: 'cssmin',
                            createConfig: function (context, block) {
                            var generated = context.options.generated;
                                generated.options = {
                                    keepSpecialComments: 0, rebase: false
                                };
                            }       
                        }]
                    }
                }
            }
        },
        concat: {
            options: {
                separator: ';'
            },
            dist: {}
        },
        uglify: {
            dist: {}
        },
        cssmin: {
            dist: {}
        },
        filerev: {
            options: {
                encoding: 'utf8',
                algorithm: 'md5',
                length: 20
            },
            release: {
                files: [{
                    src: [
                        'dist/js/*.js',
                        'dist/css/*.css'
                    ]
                }]
            }
        },
        usemin: {
            html: ['dist/list_users.html','dist/login.html','dist/menu_admin.html', 'dist/menu_user.html', 'dist/register_user.html'],
            options: {
                assetsDirs: ['dist', 'dist/css', 'dist/js']
            }
        },
        htmlmin:{
            dist: {
                options: {
                    collapseWhitespace: true
                },
                files: {
                    'dist/list_users.html': 'dist/list_users.html',
                    'dist/login.html': 'dist/login.html',
                    'dist/menu_admin.html': 'dist/menu_admin.html',
                    'dist/menu_user.html': 'dist/menu_user.html',
                    'dist/register_user.html': 'dist/register_user.html'
                }
            }
        }
        
    });

    grunt.registerTask('css', ['sass']);
    grunt.registerTask('default', ['browserSync', 'watch'])
    grunt.registerTask('build', [
        'clean',
        'copy',
        'imagemin',
        'useminPrepare',
        'concat',
        'cssmin',
        'uglify',
        'filerev',
        'usemin',
        'htmlmin'
    ])
}