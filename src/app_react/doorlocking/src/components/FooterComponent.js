import React, { Component } from "react";

class Footer extends Component {
    render(){
        return(

            <div className="footer">
                <div className="legal pure-g">
                    <div className="pure-u-1 pure-u-sm-1-2">
                        <p className="legal-license">
                            DoorLocking.App v1.0.0<br/>
                            This is free software under <a href="https://www.gnu.org/licenses/lgpl-3.0.pt-br.html">LGPL v3 license</a>.
                        </p>
                    </div>
                
                    <div className="pure-u-1 pure-u-sm-1-2">
                        <ul className="legal-links">
                            <li><a href="https://github.com/diogosmendonca/doorlocking">GitHub Project</a></li>
                            <li><a href="mailto:admin@doorlocking.app">Contact Us</a></li>
                        </ul>
                
                        <p className="legal-copyright">
                            © Copyright 2019 DoorLocking.App.
                        </p>
                    </div>
                </div>
            </div>

        );
    }
}

export default Footer;