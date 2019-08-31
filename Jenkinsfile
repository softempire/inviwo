node {
    stage('Fetch') { 
        dir('inviwo') {
            scmVars = checkout scm
            sh 'git submodule sync --recursive' // needed when a submodule has a new url  
            sh 'git submodule update --init --recursive'
        }
    }

    def util = load "${env.WORKSPACE}/inviwo/tools/jenkins/util.groovy"
    if (!env.disabledProperties) properties(util.defaultProperties())
    util.printMap("Environment", env.getEnvironment())
    
    cfg = [ errors: [], display: 0 ]

    util.wrap(this, "#jenkins-branch-pr") {
        util.touchwarn()
        util.buildStandard(
            state: this,
            modulePaths: [], 
            onModules: ["DiscreteData", "HDF5", "OpenCL", "BaseCL", "WebBrowser", "Example"],  
            offModules: ["ABufferGL"],
            opts: [:]
        )
        util.format(this, ["${env.WORKSPACE}/inviwo"])
        util.warn(this)
        util.unittest(this)
        util.integrationtest(this)        
        //util.regression(this, ["${env.WORKSPACE}/inviwo/modules"])
        util.copyright(this)    
        util.doxygen(this)
    }
}
