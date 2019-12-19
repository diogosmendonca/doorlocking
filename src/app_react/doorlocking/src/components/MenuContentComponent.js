import React, { Component } from "react";

class MenuContent extends Component {

    render(){
        return(
            <div className="content">
                <br/>
                <p className="center"><button onclick="this.className = 'pure-button button-xlarge-fixed'; location.href='/open_door';" className="button-success pure-button button-xlarge-fixed">Open Door</button></p>
                <p className="center"><button onclick="this.className = 'pure-button button-xlarge-fixed'; location.href='/register_user'" className="button-success pure-button button-xlarge-fixed">New User</button></p>
                <p className="center"><button onclick="this.className = 'pure-button button-xlarge-fixed'; location.href='/list_users'" className="button-success pure-button button-xlarge-fixed">List/Change Users</button></p>
                <p className="center"><button onclick="this.className = 'pure-button button-xlarge-fixed'; location.href='/logout_handler'" className="button-success pure-button button-xlarge-fixed">Logout</button></p>
            </div>
        );
    }
}

export default MenuContent;