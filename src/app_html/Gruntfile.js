'use strict'

module.exports = function(grunt){
    require('time-grunt')(grunt);
    require('jit-grunt')(grunt, {
        useminPrepare: 'grunt-usemin'
    });
    grunt.loadNpmTasks('grunt-contrib-compress');
    
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
            deploy_html: {
                files: [{
                    expand: true,
                    dot: true,
                    cwd: './dist/',
                    src: ['*.html'],
                    dest: '../doorlocking/data'
                }]
            }
        },
        clean: {
            build: {
                src: ['dist/']
            },
            deploy: {
                src: ['../doorlocking/data/*.html', 
                      '../doorlocking/data/*.js',
                      '../doorlocking/data/*.css', 
                      '../doorlocking/data/*.gz']
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
                src: ['list_users.html','login.html','menu_admin.html', 'menu_user.html', 'register_user.html', 'config.html']
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
                length: 5
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
            html: ['dist/list_users.html','dist/login.html','dist/menu_admin.html', 'dist/menu_user.html', 'dist/register_user.html', 'dist/config.html'],
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
                    'dist/register_user.html': 'dist/register_user.html',
                    'dist/config.html': 'dist/config.html'
                }
            }
        },
        // gzip assets 1-to-1 for production
        compress: {
            deploy_js: {
                options: {
                    mode: 'gzip'
                },
                expand: true,
                cwd: './dist/js',
                src: ['main.*.js'],
                dest: '../doorlocking/data',
                extDot: 'last',
                ext: '.js.gz'
            },
            deploy_css: {
                options: {
                    mode: 'gzip'
                },
                expand: true,
                cwd: './dist/css',
                src: ['main.*.css'],
                dest: '../doorlocking/data',
                extDot: 'last',
                ext: '.css.gz'
            }
        }
        
    });

    grunt.registerTask('css', ['sass']);
    grunt.registerTask('default', ['browserSync', 'watch'])
    grunt.registerTask('build', [
        'clean:build',
        'copy:html',
        'imagemin',
        'useminPrepare',
        'concat',
        'cssmin',
        'uglify',
        'filerev',
        'usemin',
        'htmlmin',
        'clean:deploy',
        'copy:deploy_html',
        'compress:deploy_css',
        'compress:deploy_js'
    ])
}